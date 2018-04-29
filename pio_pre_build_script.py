Import('env')

#
# Dump build environment (for debug)
print env.Dump()
#



#env.Replace(UPLOAD_PORT="192.168.0.184")
env.Replace(UPLOAD_PORT="COM4")
