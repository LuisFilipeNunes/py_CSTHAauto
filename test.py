import requests

url = "http://32.0.1.20:5000/confirm-coffee"  # Replace with the appropriate endpoint

# Make the GET request
response = requests.get(url)

# Print the response
print("Response from server:")
print(response.status_code)
print(response.text)