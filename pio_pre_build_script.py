Import('env')

#
# Dump build environment (for debug)
print env.Dump()
#



env.Replace(UPLOAD_PORT="172.17.1.22")
#env.Replace(UPLOAD_PORT="COM6")
