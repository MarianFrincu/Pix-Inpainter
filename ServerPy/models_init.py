import torch
from torch import nn
from torchvision import transforms

from models import Generator

class MappedTanh(nn.Module):
    def __init__(self):
        super().__init__()

    def forward(self, x):
        return (torch.tanh(x) + 1) / 2


def load_model(model, model_path, device):
    model = model.to(device)
    model.load_state_dict(torch.load(model_path, weights_only=True, map_location=device))
    model.eval()
    for module in model.modules():
        if isinstance(module, torch.nn.Dropout):
            module.train()
    return model


def initialize_models(device):

    transform_01 = transforms.Compose([
        transforms.Resize((256, 256)),
        transforms.ToTensor()
    ])

    transform_11 = transforms.Compose([
        transforms.Resize((256, 256)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.5], std=[0.5])
    ])

    models = {
        "sketchgan01": {
            "model": load_model(Generator(1, 1, MappedTanh()),
                                "trained_models/sketchgan01/generator.pth",
                                device),
            "transform": transform_01
        },
        "sketchgan_11": {
            "model": load_model(Generator(1, 1, torch.nn.Tanh()),
                                "trained_models/sketchgan_11/generator.pth",
                                device),
            "transform": transform_11
        },
        "wgan01": {
            "model": load_model(Generator(1, 1, MappedTanh()),
                                "trained_models/wgan01/generator.pth",
                                device),
            "transform": transform_01
        },
        "wgan_11": {
            "model": load_model(Generator(1, 1, torch.nn.Tanh()),
                                "trained_models/wgan_11/generator.pth",
                                device),
            "transform": transform_11
        }
    }

    return models
