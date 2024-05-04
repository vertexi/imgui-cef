from transformers import TrOCRProcessor
from optimum.onnxruntime import ORTModelForVision2Seq
from transformers import onnx

model_checkpoint = "breezedeus/pix2text-mfr"
save_directory = "./onnx/"

# Load a model from transformers and export it to ONNX
processor = TrOCRProcessor.from_pretrained('breezedeus/pix2text-mfr')
model = ORTModelForVision2Seq.from_pretrained('breezedeus/pix2text-mfr', use_cache=False)

# Save the onnx model and tokenizer
processor.save_pretrained(save_directory)
model.save_pretrained(save_directory)
