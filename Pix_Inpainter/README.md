# Pix Inpainter

A Qt 6 (Widgets) desktop paint / inpainting application, built with CMake.

The source code is plain Qt with no platform-specific code, so it should build
anywhere Qt 6 does. Only the Docker setup is Linux-specific.

**Tested on Linux**, both through Docker and natively. The Windows build below
follows from the same CMake project but has not been verified.


## Linux

### Option 1 — Docker (nothing to install but Docker)

```bash
./run.sh
```

This builds the image (compiling with CMake + Ninja in the container) and opens
the window on your local display. The script runs `xhost +local:docker` so the
container can reach your X server (works under Wayland too via XWayland).

If the window does not appear, make sure `xhost` is installed and run it once
manually:

```bash
xhost +local:docker
```

To build the image without running it:

```bash
docker build -f docker/Dockerfile -t pixinpainter .
```

### Option 2 — Native build

Install the dependencies (Debian/Ubuntu):

```bash
sudo apt install build-essential cmake ninja-build qt6-base-dev qt6-base-dev-tools
```

Then build and run:

```bash
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/PixInpainter
```


## Windows

`run.sh` and the Docker setup **do not work on Windows** — they rely on an X11
display socket and host networking, which are Linux-only. On Windows, build
natively instead.

You need:

* **Qt 6** (the [official Qt installer](https://www.qt.io/download-qt-installer), with the MSVC 64-bit component)
* **Visual Studio 2022** with the "Desktop development with C++" workload
* **CMake 3.21+** (bundled with Visual Studio)

### Option 1 — Command line

From a Developer Command Prompt for VS 2022, in this folder:

```bat
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.8.0/msvc2022_64"
cmake --build build --config Release
```

Adjust `CMAKE_PREFIX_PATH` to your Qt version and compiler directory. The
executable lands in `build\Release\PixInpainter.exe`.

To run it outside the build directory, copy the Qt DLLs next to it:

```bat
C:\Qt\6.8.0\msvc2022_64\bin\windeployqt.exe build\Release\PixInpainter.exe
```

### Option 2 — Visual Studio / Qt Creator

Both open a CMake project directly:

* **Visual Studio 2022**: File → Open → Folder, pick this folder. Set
  `CMAKE_PREFIX_PATH` to your Qt directory in Project → CMake Settings, then
  build and run.
* **Qt Creator**: File → Open File or Project, pick `CMakeLists.txt`, choose a
  Qt 6 kit and hit Run.

### Option 3 — WSL2

If you have WSL2, you can skip the Qt and Visual Studio install entirely.
Windows ships WSLg, which provides a display server and the `/tmp/.X11-unix`
socket that the container expects, so the Linux instructions apply unchanged:

1. Install a WSL2 distribution (e.g. Ubuntu) and Docker inside it.
2. Clone this repository **into the WSL filesystem**, not `/mnt/c`.
3. Run `./run.sh` as described in the Linux section above.

This is likely the least painful way to run the app on Windows, but it is
untested. Reports are welcomed.


## AI completion

The AI features need the Python server in [`../ServerPy`](../ServerPy) to be
running — see its README. The app works fine without it, just without AI
completion.

The server address defaults to `http://localhost:5000/`. Override it with the
`PIX_INPAINTER_SERVER_URL` environment variable:

```bash
PIX_INPAINTER_SERVER_URL=http://192.168.1.10:5000/ ./build/PixInpainter
```

Docker Compose forwards the same variable, and the container runs with
`network_mode: host`, so a server running on the host is reachable as
`localhost` from inside the container.
