for i in {1..128}; do
    dd if=/dev/urandom of=file_$i.bin bs=1000k count=1
done
