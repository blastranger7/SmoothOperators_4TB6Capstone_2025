import numpy as np
import argparse
import tensorflow as tf
import cv2
import requests
import six

from object_detection.utils import ops as utils_ops
from object_detection.utils import label_map_util
from object_detection.utils import visualization_utils as vis_util

# patch tf1 into `utils.ops`
utils_ops.tf = tf.compat.v1

# Patch the location of gfile
tf.gfile = tf.io.gfile


def load_model(model_path):
    model = tf.saved_model.load(model_path)
    return model


def run_inference_for_single_image(model, image):
    image = np.asarray(image)
    # The input needs to be a tensor, convert it using `tf.convert_to_tensor`.
    input_tensor = tf.convert_to_tensor(image)
    # The model expects a batch of images, so add an axis with `tf.newaxis`.
    input_tensor = input_tensor[tf.newaxis,...]
    
    # Run inference
    output_dict = model(input_tensor)

    # All outputs are batches tensors.
    # Convert to numpy arrays, and take index [0] to remove the batch dimension.
    # We're only interested in the first num_detections.
    num_detections = int(output_dict.pop('num_detections'))
    output_dict = {key: value[0, :num_detections].numpy()
                   for key, value in output_dict.items()}
    output_dict['num_detections'] = num_detections
    # print(num_detections)

    # detection_classes should be ints.
    output_dict['detection_classes'] = output_dict['detection_classes'].astype(np.int64)
    # print(output_dict['detection_boxes'])
   
    # Handle models with masks:
    if 'detection_masks' in output_dict:
        # Reframe the the bbox mask to the image size.
        detection_masks_reframed = utils_ops.reframe_box_masks_to_image_masks(
                                    output_dict['detection_masks'], output_dict['detection_boxes'],
                                    image.shape[0], image.shape[1])      
        detection_masks_reframed = tf.cast(detection_masks_reframed > 0.5, tf.uint8)
        output_dict['detection_masks_reframed'] = detection_masks_reframed.numpy()
    
    return output_dict


def run_inference(model, category_index, cap):
    imageWidth = 640.
    imageHeight = 640.
    calWidth = 165
    calDist = 40
    maxAngle = 13.
    calAngle = maxAngle/(imageWidth/2)
    canState = 1
    canDist = 57
    canAngle = 12
    url = "http://192.168.0.22:5000/write"
    while True:
        ret, image_np = cap.read()
        # Actual detection.
        output_dict = run_inference_for_single_image(model, image_np)
        # Visualization of the results of a detection.
        min_score_thresh = 0.4
        vis_util.visualize_boxes_and_labels_on_image_array(
            image_np,
            output_dict['detection_boxes'],
            output_dict['detection_classes'],
            output_dict['detection_scores'],
            category_index,
            instance_masks=output_dict.get('detection_masks_reframed', None),
            use_normalized_coordinates=True,
            line_thickness=2,
            min_score_thresh=min_score_thresh)
        cv2.imshow('object_detection', cv2.resize(image_np, (640, 640)))

        for i in range(output_dict['detection_boxes'].shape[0]):
            if output_dict['detection_classes'][i] in six.viewkeys(category_index):
                class_name = category_index[output_dict['detection_classes'][i]]['name']
            else:
                class_name = 'N/A'
                
            if class_name == 'soda can':
                if output_dict['detection_scores'][i] > min_score_thresh:
                    canState = 1
                    canDist = round((calWidth*calDist)/(abs(output_dict['detection_boxes'][i][1]*imageWidth - output_dict['detection_boxes'][i][3]* imageWidth)))
                    canAngle = 13
            else:
                canState = 0


        data = {'message': [canState, canDist, canAngle, 0]}

        try:
            response = requests.post(url, json=data)
            # print(f"Response Status Code: {response.status_code}")
            # print(f"Raw Response: {response.text}")  # Print raw response before parsing
            # print(f"Parsed JSON: {response.json()}")
            if response.status_code == 200:
                print(f"Status: {response.json()['message']}")
            else:
                print(f"Status: Error - {response.json()['message']}")
        except requests.exceptions.JSONDecodeError:
            print("Error: Response is not valid JSON")
        except requests.exceptions.RequestException as e:
            print(f"Request failed: {str(e)}")
        except KeyError:
            print("Error: 'message' key not found in response JSON")
        except Exception as e:
                print(f"Status: Error - {str(e)}")

        if cv2.waitKey(25) & 0xFF == ord('q'):
            cap.release()
            cv2.destroyAllWindows()
            break


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Detect objects inside webcam videostream')
    parser.add_argument('-m', '--model', type=str, required=True, help='Model Path')
    parser.add_argument('-l', '--labelmap', type=str, required=True, help='Path to Labelmap')
    args = parser.parse_args()

    detection_model = load_model(args.model)
    category_index = label_map_util.create_category_index_from_labelmap(args.labelmap, use_display_name=True)

    cap = cv2.VideoCapture('http://192.168.0.22:7123/stream.mjpg')
    run_inference(detection_model, category_index, cap)

# python .\detect_from_webcam.py -m ssd_mobilenet_v2_320x320_coco17_tpu-8\saved_model -l .\data\mscoco_label_map.pbtxt tcp/h264://192.168.1.90:8554