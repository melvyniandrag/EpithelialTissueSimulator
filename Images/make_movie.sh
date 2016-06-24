cd Images
python format_script.py
geomview -c script.txt
convert *.rgb movie.gif
rm *.rgb 
