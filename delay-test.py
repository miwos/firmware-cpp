Import("env")

def after_upload(source, target, env):
  if "UNIT_TEST" in env["CPPDEFINES"]:
    import time
    time.sleep(5)

env.AddPostAction("upload", after_upload)