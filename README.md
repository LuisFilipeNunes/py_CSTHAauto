Coffee Machine Control System

Overview

This project consists of a Flask web application and an Arduino setup that work together to control a coffee machine. The web application provides a user interface for setting coffee brewing times and checking machine status, while the Arduino handles the physical control of the coffee machine based on the received commands.

Project Structure

    app.py: Main Python file containing the Flask application code.
    templates/: Folder containing HTML templates for the web interface.
    static/: Folder containing static files (e.g., CSS stylesheets).
    coffee_data.json: Stores coffee settings (brewing time, shutdown time, etc.).
    coffee_system.html: HTML template for the coffee machine control interface.
    coffee_style.css: CSS stylesheet for the coffee control interface.
    arduino_code.ino: Arduino code for interacting with the coffee machine.

Running the Project

    Install Dependencies:
        Python 3 with Flask, datetime, and json libraries.
        Arduino IDE with ArduinoJson library.

    Start the Flask Server:

    flask run --host=A.B.C.D

    However, i would recommend using gunicorn for better stability on low-end server/computers. 

    Upload Arduino Code:
        Connect the Arduino to your computer.
        Open arduino_code.ino in the Arduino IDE.
        Adjust network settings (IP addresses, MAC address) if needed.
        Upload the code to the Arduino board.

    Access the Web Interface:
        Open a web browser and go to http://10.0.11.167:5007/coffee_system (or the IP address where the Flask server is running).

Usage

    Set Coffee Time:
        Enter the desired brewing time in the "Hora para Ligar" field.
        Optionally, enter a shutdown time in the "Hora de Desligamento" field.
        Click the "Programar café" button.


Additional Features

    "Cafeteira foi preenchida?" button: Indicates if the coffee machine is filled.
    "Controle Manual" button: Enables manual control of the coffee machine.

Key Components

    Flask: Web framework for handling HTTP requests and serving HTML content.
    Arduino: Microcontroller board for controlling the coffee machine's hardware, connected to a ehternet shield and two relays.
    JSON: Data format used for communication between the web application and Arduino.

Future Updates:

    Control for room light - Using relays.
    Control AC Unit - Using IR IC.

Author:

    LuisFilipeNunes

Date:

    February 23, 2024



