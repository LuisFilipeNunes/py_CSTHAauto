# Import necessary libraries
import os, json 
from datetime import datetime, timedelta
from flask import Flask, render_template, request, jsonify

# Initialize Flask application
app = Flask(__name__)

# Configuration notes for running the Flask application
# - Use `flask run --host=0.0.0.0` to make the app publicly accessible on your network.
# - Avoid using `--debug` flag in production environments due to security concerns.

# Define the path to the data file storing coffee settings
DATA_FILE_PATH = 'coffee_data.json'

# Check if the data file exists. If not, create an empty one.
if not os.path.exists(DATA_FILE_PATH):
    with open(DATA_FILE_PATH, 'w') as file:
        file.write('{}') 

# Route for the main page
@app.route("/")
def index():
    """
    This route renders the main HTML template, containing the overall interface.
    """
    return render_template('index.html')

# Route for the coffee system interface
@app.route('/coffee_system')
def coffee_system():
    """
    This route renders the specific HTML template for the coffee system interface.
    """
    return render_template('coffee_system/coffeepot.html')

# Route for light system (unimplemented)
@app.route('/light_system')
def light_system():
    """
    This route renders the specific HTML template for the light system interface.
    """
    return render_template('light_system/light.html')

# Route for ac system (unimplemented)
@app.route('/ac_system')
def ac_system():
    """
    This route renders the specific HTML template for the ac system interface.
    """
    return render_template('ac_system/conditioner.html')

# Route for confirming coffee settings
@app.route('/confirm-coffee', methods=['POST'])
def confirm_coffee():
    """
    This route handles POST requests sent when confirming coffee settings.
    - Retrieves the JSON data sent with the request.
    - Prints the received data for debugging/logging purposes.
    - Writes the received data (coffee settings) to the data file.
    - Returns a confirmation message.
    """
    global repeat_daily 

    data = request.get_json()
    print(data)  # Print received data for debugging/logging

    with open(DATA_FILE_PATH, 'w') as f:
        f.write(json.dumps(data))   

    return "Coffee Confirmed!"

# Route for retrieving coffee data
@app.route('/get-coffee-data', methods=['GET'])
def get_coffee_data():
    """
    This route handles GET requests for retrieving coffee data from the data file.
    - Reads the data from the file.
    - Checks if the data contains a "shutdownTime" key.
    - If missing, calculates the shutdown time based on the "coffeeTime" key (assuming a 2-hour window).
    - Returns the data in JSON format.
    """
    with open(DATA_FILE_PATH, 'r') as file:
        stored_data = json.load(file)  # Load data from the file

    if "shutdownTime" not in stored_data or not stored_data["shutdownTime"]:
        try:
            # Extract and parse the coffee time from the data
            coffee_time_str = stored_data['coffeeTime']
            coffee_time_obj = datetime.strptime(coffee_time_str, "%H:%M")
        except ValueError:
            print("Error parsing coffeeTime:", coffee_time_str)
            return "Invalid coffeeTime format."

        # Calculate the shutdown time based on coffee time (assuming 2-hour window)
        shutdown_time_obj = coffee_time_obj + timedelta(hours=2)
        shutdown_time_str = shutdown_time_obj.strftime("%H:%M")
        stored_data["shutdownTime"] = shutdown_time_str  # Update the data with calculated shutdown time

    # Return the coffee data as JSON
    return jsonify(stored_data)


# Run the Flask application
if __name__ == '__main__':
    app.run(debug=False, host='10.0.11.167', port=5007)
