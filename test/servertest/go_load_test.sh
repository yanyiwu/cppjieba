# go get github.com/yanyiwu/go_http_load
go_http_load -method=GET -get_urls="../test/testdata/load_test.urls" -loop_count=500 -goroutines=2
