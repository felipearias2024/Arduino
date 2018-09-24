import urllib2
import json
import glob
from collections import defaultdict

fileNames = defaultdict(list)

# fill sorted list of tables based on svg filenames
svgFiles = sorted(glob.glob('*.svg'))
for s in svgFiles: 
    fileNames[s.split('.', 1)[0]].append(s) 


req = urllib2.Request('http://192.168.30.240')
opener = urllib2.build_opener()
f = opener.open(req)
json = json.loads(f.read())
a = json["0"]["TemperaturaInterior"]
b = json["0"]["TemperaturaExterior"]
	
html = """<html><table border="1">
<tr><th>Humedad</th><th>Temperatura</th></tr>"""
html += "<tr><td>{}</td><td>{}</td>".format(b, a)
html += "</tr>"
html += "</table></html>"

file_ = open('result.html', 'w')
file_.write(html)
file_.close()

#print json['unit']
