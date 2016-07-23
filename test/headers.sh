for i in `ls *.h`; do
	echo "#include \"$i\"" >> all_headers.h; 
done
