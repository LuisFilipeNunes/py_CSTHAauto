from flask import Flask, render_template, request

app = Flask(__name__)
# flask run --host=0.0.0.0 to be public to everyone in the network.
#run --debug this flag means it will be more responsive, but i lil more insecure. Dont do it on open networks or in production state. 


repeat_daily = False  # Initialize the repeat flag 


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

    if repeat_daily:
        print(f"Confirmed Coffee Time: {coffee_time}, keeping the configuration")
    else:
        print(f"Confirmed Coffee Time: {coffee_time}, repeat_daily is False")

    return "Coffee Confirmed!"

if __name__ == '__main__':
    app.run(debug=True)
