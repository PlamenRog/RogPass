<pre style="font-size: 2em; font-weight: bold; margin-bottom: 0.5em; text-align:center">
______                ______                  
| ___ \               | ___ \                 
| |_/ /  ___    __ _  | |_/ /__ _  ___  ___ 
|    /  / _ \  / _` | |  __// _` |/ __|/ __|
| |\ \ | (_) || (_| | | |  | (_| |\__ \\__ \
\_| \_| \___/  \__, | \_|   \__,_||___/|___/
                __/ |                        
               |___/                         
</pre>


<p style="text-align:center"><em>A somewhat safe password manager written in c</em></p>


Dependencies:
 - GCC (tested on version 11.4 and 12.2 with c18 and c2x flags, but should work on older ones as well)
 - OpenSSL (>=3.0.0)

Compile it with:
```make
make install
```

Recommended way to run is to put the binary somewhere in your PATH and then do
```bash
rogp
```
where exactly you will put the binary is entirely up to you, of course.