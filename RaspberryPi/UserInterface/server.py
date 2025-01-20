from flask import Flask, request, jsonify
import serial

app = Flask(__name__)

# Initialize UART
ser = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

API_KEYS = {
    "object_detection_system": "ods_key",
    "robot_software": "rs_key"
}

# Define the endpoint to receive data
@app.route('/write', methods=['POST'])
def write_to_file():
    try:
        # Get JSON data from the request
        data = request.json
        message = data.get('message', 'No message provided')
        
        # Write data to a text file to track changes
        with open('received_data.txt', 'w') as file:
            file.write(message + '\n')

        # Send serial data to microcontroller
        ser.write(message.encode('utf-8'))

        # Indicate that state of the can should be updated
        with open('updated_can_state.txt', 'w') as can_file:
            can_file.write("Update" + '\n')
        
        return {'status': 'success', 'message': 'Data written to file'}, 200
    except Exception as e:
        return {'status': 'error', 'message': str(e)}, 500
    
# Define the endpoint to transmit data
@app.route('/read', methods=['GET'])
def read_from_file():
    
    # Check that API key matches one of the values in dictionary above
    api_key = request.headers.get('API-Key')
    if api_key not in API_KEYS.values():
        return jsonify({'status': 'error', 'message': 'Unauthorized'}), 401

    if api_key == API_KEYS["object_detection_system"]:
        try:
            with open('updated_can_state.txt', 'r+') as can_file:
                state = can_file.read()

                # Reset the file back to not updated
                can_file.seek(0)
                can_file.truncate(0)
                can_file.write("No update" + '\n')

                return jsonify({'status': 'success', 'data': state}), 200
        except FileNotFoundError:
            return {'status': 'error', 'message': 'File not found'}, 404
        except Exception as e:
            return {'status': 'error', 'message': str(e)}, 500

    elif api_key == API_KEYS["robot_software"]:
        try:
            # Read data from the text file
            with open('received_data.txt', 'r') as file:
                lines = file.readlines()
            
            # Convert lines into a JSON response
            return jsonify({'status': 'success', 'data': lines}), 200
        except FileNotFoundError:
            return {'status': 'error', 'message': 'File not found'}, 404
        except Exception as e:
            return {'status': 'error', 'message': str(e)}, 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)