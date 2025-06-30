import cv2
import torch
import numpy as np

def apply_postprocess(tensor: torch.Tensor) -> torch.Tensor:
    min_val = tensor.min().item()
    max_val = tensor.max().item()

    is_normalized = (min_val < 0)

    if is_normalized:
        image = tensor.mul(0.5).add(0.5).clamp(0, 1).detach().cpu().numpy()
    else:
        image = tensor.clamp(0, 1).detach().cpu().numpy()

    image = (image[0] * 255).astype(np.uint8)

    var_d = 7
    var_r = 125
    image_filtered = cv2.bilateralFilter(image, d=var_d, sigmaColor=var_r, sigmaSpace=var_r)

    mask_size = 15
    coef = 0.9
    C = int((1 - coef) * 255)

    thresholded = cv2.adaptiveThreshold(
        image_filtered,
        255,
        cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
        cv2.THRESH_BINARY,
        blockSize=mask_size,
        C=C
    )

    thresholded_tensor = torch.from_numpy(thresholded.astype(np.float32) / 255.0).unsqueeze(0)

    if is_normalized:
        thresholded_tensor = thresholded_tensor * 2 - 1

    return thresholded_tensor
