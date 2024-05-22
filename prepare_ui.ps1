mkdir build
mkdir wat-ui/wat-bin
make
mv build/output* wat-ui/wat-bin/
echo "Run 'cd wat-ui && npm install && npm start' to start the UI."