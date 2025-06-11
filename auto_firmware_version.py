import subprocess

Import("env")

def get_firmware_specifier_build_flag():    
    ret_tag = subprocess.run(["git", "describe", "--tags", "--abbrev=0"], stdout=subprocess.PIPE, text=True)
    ret_commit = subprocess.run(["git", "describe", "--always"], stdout=subprocess.PIPE, text=True)
    tag = ret_tag.stdout.strip()
    commit = ret_commit.stdout.strip()
    build_flag = "-D AUTO_VERSION=\\\"" + tag + "-" + commit + "\\\""
    print ("Firmware Revision: " + tag)
    return (build_flag)

env.Append(
    BUILD_FLAGS=[get_firmware_specifier_build_flag()]
)
