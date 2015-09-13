# GP TEE seminar exercise
This set of files are the ones used at the annual [GlobalPlatform TEE seminar](http://www.teeseminar.org/about_the_workshop.asp)
([TEE Application Developers Workshop](http://www.globalplatform.org/TEEevent/about_the_workshop.asp)) for
the session held by Linaro.

The git contains all files necessary to build a host (normal world user space
client) application and also a Trusted Application. The files located in the
`host` and `ta` folder are skeleton/template files where the attendee(s) are
supposed to fill in this missing pieces.

There is also a folder called `solution`, which contains the exact same set of
files, however there you will find that all functions are fully implemented. The
attendees are only supposed to have that as a last resort in case they get
stuck.

To build the binaries we have provided a little shell-script
(`build-gp-conf.sh`) which setup the environment variables to the toolchain,
ta-dev-kit, Client API etc. Beware that this will output the files outside this
particular git (`../out/gp_conf/`). The reason for this is because the attendees
are supposed to have the environment setup according to the instructions for
OP-TEE and QEMU (see [6. repo manifests](https://github.com/OP-TEE/optee_os/#6-repo-manifests)).

In the `solution/patch` folder you will find two simple patches:

- `build/0001-Adds-gp_conf-ta-files-to-root-fs.patch`: Applies on the
[build](https://github.com/OP-TEE/build) git. It just adds the lines which are
needed to get the binaries included in the root FS used by QEMU.
- `gen_rootfs/0001-Adding-a-modprobe-alias-for-quick-loading.patch`: Just
contains an `alias mp` which save a few keystrokes at every boot (it loads optee
kernel modules and tee-supplicant in one go). This patch applies to the
[gen_rootfs](https://github.com/linaro-swg/gen_rootfs) git.
