import sys

bin_path = sys.argv[1]
ini_path = sys.argv[2]
res_path = sys.argv[3]

with open(bin_path, 'rb') as f: 
    binary = f.read()
    
cfg = {}
with open(ini_path, 'r') as f:
    for line in f.read().splitlines():
        k,v = tuple(map(lambda s:s.strip(), line.split('=')))[:2]        
        cfg[k]=v

got_start = int(cfg["got_start"], 16)
got_length = int(cfg["got_length"], 16)
got_end = got_start + got_length

with open(res_path, 'wb') as f:
    f.write(b"PICx")
    f.write(got_start.to_bytes(4, 'little'))
    f.write(got_end.to_bytes(4, 'little'))
    f.write(len(binary).to_bytes(4, 'little'))
    f.write(binary)
