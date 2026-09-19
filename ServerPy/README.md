# Pix Inpainter — AI Server

A small Flask server that runs the sketch-completion GAN models and exposes them
over a REST API. The desktop app in [`../Pix_Inpainter`](../Pix_Inpainter) talks
to this server for its AI completion features.

## Setup

1. Install the dependencies:

```bash
pip install flask numpy opencv-python pillow torchvision
```

For PyTorch, follow the official guide: [PyTorch Installation](https://pytorch.org/get-started/locally/)

2. Add the trained model weights under `trained_models/`, one folder per model:

```
trained_models/
├── sketchgan01/generator.pth
├── sketchgan_11/generator.pth
├── wgan01/generator.pth
└── wgan_11/generator.pth
```

The weights are **not included in this repository**. They come from the
[Sketch Completion With GANs](https://github.com/MarianFrincu/Sketch-Completion-With-GANs)
project. The server will not start without them.

3. Run the server:

```bash
python run.py
```

It listens on port `5000` and uses CUDA when available, otherwise the CPU.

## API

| Endpoint   | Method | Description                                      |
| ---------- | ------ | ------------------------------------------------ |
| `/models`  | GET    | Lists the available model keys                   |
| `/process` | POST   | Completes the posted PNG with one model          |
| `/compare` | POST   | Same as `/process`, used for comparing models    |

`/process` and `/compare` take the raw PNG as the request body and the
`model_id` and `postprocess_value` query parameters, and return a PNG.
