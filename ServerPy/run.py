from flask import Flask, request, Response
from io import BytesIO
from PIL import Image

import torch
from torchvision import transforms
from postprocess_func import apply_postprocess
from models_init import initialize_models
app = Flask(__name__)

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

transform_to_pil = transforms.ToPILImage()

models = initialize_models(device)

def handle_request(model_key: str, postprocess_value: int, image_bytes: bytes):
    print(f"[INFO] Requested model: {model_key}, Postprocess: {postprocess_value}")

    image_data = BytesIO(image_bytes)
    original_image = Image.open(image_data).convert("L")
    original_size = original_image.size

    model = models.get(model_key)
    if model is None:
        raise ValueError(f"Model key '{model_key}' not found.")

    gen = model["model"]
    transform = model["transform"]

    image = transform(original_image).unsqueeze(0).to(device)

    was_normalized = (image.min() < 0)

    with torch.no_grad():
        if postprocess_value > 0:
            for i in range (postprocess_value):
                image = gen(image)
                image = apply_postprocess(image.squeeze(0).cpu()).unsqueeze(0).to(device)
        else:
            image = gen(image)

    image = image.squeeze(0).cpu()

    if was_normalized:
        image = (image + 1) / 2

    output_image = transform_to_pil(image)
    output_image = output_image.resize(original_size)

    output = BytesIO()
    output_image.save(output, format="PNG")
    output.seek(0)

    return Response(output.getvalue(), mimetype='image/png')


@app.route('/process', methods=['POST'])
def process_image():
    try:
        model_key = request.args.get("model_id")
        postprocess_value = int(request.args.get("postprocess_value", 0))
        return handle_request(model_key, postprocess_value, request.data)
    except Exception as e:
        import traceback
        traceback.print_exc()
        return f"Error: {str(e)}", 500


@app.route('/compare', methods=['POST'])
def compare_image():
    try:
        model_key = request.args.get("model_id")
        postprocess_value = int(request.args.get("postprocess_value", 0))
        return handle_request(model_key, postprocess_value, request.data)
    except Exception as e:
        import traceback
        traceback.print_exc()
        return f"Error: {str(e)}", 500

@app.route('/models', methods=['GET'])
def list_models():
    try:
        model_keys = list(models.keys())
        return {"available_models": model_keys}, 200
    except Exception as e:
        import traceback
        traceback.print_exc()
        return f"Error: {str(e)}", 500


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=False)