valgrind --tool=callgrind /usr/local/bin/qemu-system-x86_64 \
  -m 1G \
  -smp 1 \
  -cpu host,vmx=on \
  -kernel $KERNEL \
  -append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0" \
  -drive file=$IMAGE/stretch.img,format=raw \
  -net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10021-:22 \
  -net nic,model=e1000 \
  -enable-kvm \
  -nographic \
  -pidfile vm.pid \
  2>&1 | tee vm.log
