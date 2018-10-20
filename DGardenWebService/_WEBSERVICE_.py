from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import json


hasData = False
PlantDataJson = ''

class DGardenWebHandler(BaseHTTPRequestHandler):

    hasData = False
    plantDataJson = ''


    def _set_headers(self,response_code):
        self.send_response(response_code)
        self.send_header('Content-type', 'application/json')
        self.end_headers()

    def do_GET(self):

        self._set_headers(200)
        global hasData
        global PlantDataJson

        if (hasData == True):
            self.wfile.write(json.dumps(plantDataJson,sort_keys=True))
        else:
            self.wfile.write("hi there! \n")
            self.wfile.write('There is no data available')

    def do_HEAD(self):
        self._set_headers(200)

    def do_POST(self):

        content_length = int(self.headers['Content-Length'])  # <--- Gets the size of data
        global hasData
        if content_length > 0:
            hasData =True
        post_data = self.rfile.read(content_length)  # <--- Gets the data itself
        self._set_headers(200)
        self.wfile.write( post_data )
        global plantDataJson
        plantDataJson = json.loads(post_data)

        self.wfile.write('\n')
        self.wfile.write('\n')


        #self.wfile.write(self.PlantDataJson[2]['age'])


def run(server_class=HTTPServer, handler_class=DGardenWebHandler, port=8000):
    server_address = ('0.0.0.0.', port)
    httpd = server_class(server_address, handler_class)
    print 'Starting httpd...'
    httpd.serve_forever()


if __name__ == "__main__":
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()