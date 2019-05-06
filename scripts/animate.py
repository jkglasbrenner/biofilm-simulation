#!/usr/bin/env python
"""Generate animation for biofilm simulation."""

import matplotlib

matplotlib.use("Agg")

import argparse
import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib.colors import LinearSegmentedColormap
import numpy as np
import pandas as pd


def read_sim_history(datafile: str, num_rows: int, num_columns: int) -> pd.DataFrame:
    """Read and extract from simulation datafile.

    Parameters
    ----------
    datafile : str
        File containing biofilm simulation history.

    num_rows : int
        Number of rows in simulation.

    num_columns : int
        Number of columns in simulation.

    Returns
    -------
    biofilm_history : pd.DataFrame
        Data frame of biofilm simulation history.
    """
    biofilm_history: pd.DataFrame = pd.read_csv(datafile)
    biofilm_history["row"] = biofilm_history["cell_id"] // num_columns
    biofilm_history["column"] = biofilm_history["cell_id"] % num_columns

    return biofilm_history


def build_animation(
    biofilm_history: pd.DataFrame,
    dpi: float,
    interval: int,
    fig_width: float = 8,
    fig_height: float = 4,
) -> animation.FuncAnimation:
    """Animate the biofilm simulation history data using Matplotlib.

    Parameters
    ----------
    biofilm_history : pd.DataFrame
        Data frame of biofilm simulation history.

    dpi : float, optional
        Animation resolution in dots per inch

    interval: int
        Set sampling interval for animation.

    fig_width : float, optional
        Figure width in inches

    fig_height : float, optional
        Figure height in inches

    Returns
    -------
    anim : animation.FuncAnimation
        Animated object for the simulation run.
    """
    number_rows: int = len(biofilm_history["row"].unique())
    number_columns: int = len(biofilm_history["column"].unique())
    number_frames: int = len(biofilm_history["step"].unique())

    bacteria_df: pd.DataFrame = (
        biofilm_history.query("step == 0")
        .loc[:, ["row", "column", "bacterium_state"]]
        .pivot("row", "column", "bacterium_state")
    )

    nutrients_df: pd.DataFrame = (
        biofilm_history.query("step == 0")
        .loc[:, ["row", "column", "nutrient_state"]]
        .pivot("row", "column", "nutrient_state")
    )

    fig, ax = plt.subplots(figsize=(fig_width, fig_height), ncols=2, dpi=dpi)

    cmap = LinearSegmentedColormap.from_list(
        "biofilm", [(0, "#deebf7"), (0.5, "#3182bd"), (1, "#9ecae1")]
    )

    im_bacteria = ax[0].imshow(
        bacteria_df.values, cmap=cmap, interpolation="nearest", vmin=0, vmax=2
    )

    im_nutrients = ax[1].imshow(
        nutrients_df.values, cmap="viridis", interpolation="nearest", vmin=0, vmax=1
    )

    xticks_start_bac, xticks_end_bac = ax[0].get_xlim()
    yticks_start_bac, yticks_end_bac = ax[0].get_ylim()
    xticks_start_nutrient, xticks_end_nutrient = ax[1].get_xlim()
    yticks_start_nutrient, yticks_end_nutrient = ax[1].get_ylim()

    ax[0].xaxis.set_ticks(
        np.linspace(xticks_start_bac, xticks_end_bac, number_columns + 1), minor=False
    )
    ax[0].yaxis.set_ticks(
        np.linspace(yticks_start_bac, yticks_end_bac, number_rows + 1), minor=False
    )

    ax[1].xaxis.set_ticks(
        np.linspace(xticks_start_nutrient, xticks_end_nutrient, number_columns + 1),
        minor=False,
    )
    ax[1].yaxis.set_ticks(
        np.linspace(yticks_start_nutrient, yticks_end_nutrient, number_rows + 1),
        minor=False,
    )

    ax[0].axes.grid(False, which="major")
    ax[1].axes.grid(False, which="major")

    ax[0].tick_params(labelbottom=False, labelleft=False, bottom=False, left=False)
    ax[1].tick_params(labelbottom=False, labelleft=False, bottom=False, left=False)

    ax[0].set_title("Bacteria", fontname="DejaVu Sans")
    ax[1].set_title("Nutrients", fontname="DejaVu Sans")

    def init():
        im_bacteria.set_array(np.array([[]]))
        im_nutrients.set_array(np.array([[]]))

        return (im_bacteria, im_nutrients)

    def animate(frame):
        bacteria_df = (
            biofilm_history.query(f"step == {frame}")
            .loc[:, ["row", "column", "bacterium_state"]]
            .pivot("row", "column", "bacterium_state")
        )
        nutrients_df = (
            biofilm_history.query(f"step == {frame}")
            .loc[:, ["row", "column", "nutrient_state"]]
            .pivot("row", "column", "nutrient_state")
        )
        im_bacteria.set_array(bacteria_df.values)
        im_nutrients.set_array(nutrients_df.values)

        return (im_bacteria, im_nutrients)

    plt.tight_layout()
    plt.close()

    anim: animation.FuncAnimation = animation.FuncAnimation(
        fig=fig,
        func=animate,
        frames=number_frames,
        init_func=init,
        interval=interval,
        blit=True,
    )

    return anim


def animate_simulation(
    datafile: str,
    num_rows: int,
    num_columns: int,
    writer: str = "imagemagick",
    dpi: int = 150,
    fps: int = 30,
    interval: int = 30,
) -> None:
    """Read data file and generate an animation.

    Parameters
    ----------
    datafile : str
        File containing biofilm simulation history.

    num_rows : int
        Number of rows in simulation.

    num_columns : int
        Number of columns in simulation.

    writer : str, optional
        Animation writer, 'imagemagick' outputs an animated gif image and
        'ffmpeg' outputs an mp4 movie file.

    dpi : float, optional
        Animation resolution in dots per inch

    fps: int
        Set frames per second for rendered animation (ffmpeg only).

    interval: int
        Set sampling interval for animation.
    """
    biofilm_history: pd.DataFrame = read_sim_history(datafile, num_rows, num_columns)
    anim: animation.FuncAnimation = build_animation(
        biofilm_history, dpi=dpi, interval=interval
    )

    if writer == "imagemagick":
        anim.save("biofilm_animation.gif", dpi=dpi, writer="imagemagick")

    elif writer == "ffmpeg":
        anim.save("biofilm_animation.mp4", dpi=dpi, writer="ffmpeg", fps=fps)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Animate the biofilm simulation.")
    parser.add_argument(
        "datafile", type=str, help="File containing biofilm simulation history."
    )
    parser.add_argument("rows", type=int, help="Number of rows in simulation.")
    parser.add_argument("columns", type=int, help="Number of columns in simulation.")
    parser.add_argument(
        "--writer",
        type=str,
        default="imagemagick",
        choices=set(("imagemagick", "ffmpeg")),
        help=(
            "Specify animation writer. "
            '"imagemagick" outputs an animated gif image and "ffmpeg" outputs '
            "an mp4 movie file."
        ),
    )
    parser.add_argument(
        "--dpi",
        type=int,
        default=300,
        help="Specify animation resolution in dots per inch.",
    )
    parser.add_argument(
        "--interval",
        type=int,
        default=30,
        help="Specify sampling interval for animation.",
    )
    args = parser.parse_args()

    animate_simulation(
        datafile=args.datafile,
        num_rows=args.rows,
        num_columns=args.columns,
        writer=args.writer,
        dpi=args.dpi,
        interval=args.interval,
    )
