#! /usr/bin/python

# usage put: python3 cl1p.py my_clipboard my_info 30
# usage get: curl -X GET https://api.cl1p.net/my_clipboard

import sys
import requests

clipboard_name = sys.argv[1]
clipboard_contents = sys.argv[2]
lifetime_in_minutes = sys.argv[3]

r = requests.post(url='https://cl1p.net/'+clipboard_name,
                  data={
                      'content': clipboard_contents,
                      'ttl': lifetime_in_minutes,
                  })
