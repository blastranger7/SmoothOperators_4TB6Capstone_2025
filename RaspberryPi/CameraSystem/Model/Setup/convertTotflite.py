# Convert exported graph file into TFLite model file
import tensorflow as tf

converter = tf.lite.TFLiteConverter.from_saved_model('../ssd_mobilenet_v2_320x320_coco17_tpu-8-32/saved_model')
tflite_model = converter.convert()

with open('../test2.tflite', 'wb') as f:
  f.write(tflite_model)