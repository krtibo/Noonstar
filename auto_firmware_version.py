import subprocess

Import("env")

def get_firmware_specifier_build_flag():    
    # ret = subprocess.run(["git", "describe"], stdout=subprocess.PIPE, text=True) #Uses only annotated tags
    ret = subprocess.run(["git", "describe", "--tags"], stdout=subprocess.PIPE, text=True) #Uses any tags
    ret_commit = subprocess.run(["git", "describe", "--always"], stdout=subprocess.PIPE, text=True) #Uses any tags
    build_version = ret.stdout.strip()
    commit = ret_commit.stdout.strip()
    build_flag = "-D AUTO_VERSION=\\\"" + build_version + "-" + commit + "\\\""
    print ("Firmware Revision: " + build_version)
    return (build_flag)

env.Append(
    BUILD_FLAGS=[get_firmware_specifier_build_flag()]
)
