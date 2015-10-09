#!/bin/bash
port="/dev/ttyUSB0"
# same tty magic phrase used by arduino
stty_arg="10:0:8bd:0:3:1c:7f:15:4:0:0:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0"
/bin/stty --file=$port $stty_arg
