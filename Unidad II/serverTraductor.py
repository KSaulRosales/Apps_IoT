import requests
from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import os
from dotenv import load_dotenv

load_dotenv()

X_RAPIDAPI_KEY = os.getenv("X_RAPIDAPI_KEY")

url = "https://google-translate1.p.rapidapi.com/language/translate/v2"

contador = 11


class MyHTTPRequestHandler(BaseHTTPRequestHandler):

    def _set_response(self, content_type="text/plain"):
        self.send_response(200)
        self.send_header("Content-type", content_type)
        self.end_headers()

    def throw_custom_error(self, message):
        self._set_response("application/json")
        self.wfile.write(json.dumps({"message": message}).encode())

    def do_GET(self):
        self._set_response()
        respuesta = "El valor es: " + str(contador)
        self.wfile.write(respuesta.encode())

    def do_POST(self):
        content_length = int(self.headers["Content-Length"])
        post_data = self.rfile.read(content_length)

        try:
            body_json = json.loads(post_data.decode())
        except:
            self.throw_custom_error("Invalid JSON")
            return

        if 'text' not in body_json:
            self.throw_custom_error("Missing 'text' in JSON")
            return

        payload = {
            "q": body_json['text'],
            "target": "en",  # Traducir al inglés
            "source": "es"   # Del español
        }

        headers = {
            "content-type": "application/x-www-form-urlencoded",
            "Accept-Encoding": "application/gzip",
            "X-RapidAPI-Key": X_RAPIDAPI_KEY,
            "X-RapidAPI-Host": "google-translate1.p.rapidapi.com"
        }

        response = requests.post(url, data=payload, headers=headers)

        if response.status_code == 200:
            translation = response.json().get('data', {}).get(
                'translations', [{}])[0].get('translatedText', '')
            response_data = json.dumps(
                {"message": "Translation successful", "translation": translation})
            self._set_response("application/json")
            self.wfile.write(response_data.encode())
        else:
            self.throw_custom_error("Translation failed")


def run_server(server_class=HTTPServer, handler_class=MyHTTPRequestHandler, port=7800):
    server_address = ("", port)
    httpd = server_class(server_address, handler_class)
    print(f"Starting server on port {port}...")
    httpd.serve_forever()


if __name__ == "__main__":
    run_server()
