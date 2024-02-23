import os, json 
from datetime import datetime
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)
# flask run --host=0.0.0.0 to be public to everyone in the network.
# run --debug this flag means it will be more responsive, but i lil more insecure. Dont do it on open networks or in production state.  

DATA_FILE_PATH = 'coffee_data.json'

if not os.path.exists(DATA_FILE_PATH):
    with open(DATA_FILE_PATH, 'w') as file:
        file.write('{}')

@app.route("/")
def index():
    return render_template('index.html')

@app.route('/coffee_system')
def coffee_system():
    return render_template('coffee_system/coffeepot.html')

@app.route('/light_system')
def light_system():
    return render_template('light_system/light.html')

@app.route('/ac_system')
def ac_system():
    return render_template('ac_system/conditioner.html')

@app.route('/confirm-coffee', methods=['POST'])
def confirm_coffee():
    global repeat_daily  # Use the global variable

    data = request.get_json()
    coffee_time = data.get('time')
    repeat_daily = data.get('repeatFlag')
    if repeat_daily:
        print(f"Confirmed Coffee Time: {coffee_time}, keeping the configuration")
    else:
        print(f"Confirmed Coffee Time: {coffee_time}, repeat_daily is False")
    print (data)
    
    with open(DATA_FILE_PATH, 'w') as f:
        f.write(json.dumps(data))    
    
    return "Coffee Confirmed!"

@app.route('/get-coffee-data', methods=['GET'])
def get_coffee_data():
    # Read the data from the JSON file
    with open(DATA_FILE_PATH, 'r') as file:
        stored_data = json.load(file)
    rt = datetime.now()    
    stored_data['realTime'] = rt.strftime('%H:%M')

    if stored_data:
        # Send the stored data as a JSON responseW
        return jsonify(stored_data)
    else:
        return "No data available."

if __name__ == '__main__':
   app.run(debug=False, host='10.0.11.167', port=5000)
