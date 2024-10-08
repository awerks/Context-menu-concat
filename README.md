# Concatenate Videos Utility

## Overview

The Concatenate Videos Utility is a simple command-line tool written in C that allows users to concatenate multiple video files into a single output file using FFmpeg. Additionally, it includes functionality to add a custom context menu entry in Windows Explorer for easy access.

## Features

- Concatenate multiple video files into one.
- Automatically generates a temporary file listing the input video files.
- Supports adding an audio file to the video if at least one video is provided.
- Integrates with Windows context menu for quick access.

## Requirements

- **FFmpeg**: Ensure that FFmpeg is installed and accessible from the command line. You can download it from [FFmpeg's official website](https://ffmpeg.org/download.html).
- **Windows**: This utility includes a Windows registry script to create a context menu entry.

## Usage

### Compiling the Code

1. Make sure you have a C compiler installed (e.g., GCC).
2. Compile the C program using the following command:

   ```bash
   gcc -o concat.exe concat.c
   ```

## Installing

Simply run `setup.bat` in the command prompt. This script will create the necessary registry entries to add "Concatenate Videos" to the context menu.
