cmake -Bbuild
cd build
make
cd -

echo "Aprender"
echo "--------"
./aprender.sh build/aprender ../images/imagenesL3

if [ $? -ne 0 ]; then
    exit 1
fi

echo
echo "Reconocer"
echo "---------"
for reco in ../images/imagenesL3/reco*; do
    build/reconocer $reco
done
