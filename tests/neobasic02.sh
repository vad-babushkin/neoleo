OFILE=neobasic02.dsv
neoleo -H >out/$OFILE <<EOF
# test neobasic cmd function

.BAS
cmd("goto-cell b2")
cmd("edit-cell 42")
.XBAS

type-dsv
q
EOF

diff out/$OFILE verified/$OFILE
exit $?
