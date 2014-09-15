CURL_RES=../test/testdata/curl.res
TMP=curl.res.tmp
curl -s "http://127.0.0.1:11200/?key=南京市长江大桥" >> $TMP
if diff $TMP $CURL_RES >> /dev/null
then
    echo "ok";
else
    echo "failed."
fi

rm $TMP
