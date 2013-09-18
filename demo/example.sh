./segment_demo testlines.gbk --encoding gbk --dictpath ../dicts/jieba.dict.gbk
./segment_demo testlines.gbk --modelpath ../dicts/hmm_model.gbk --algorithm cutHMM
./segment_demo testlines.gbk --modelpath ../dicts/hmm_model.gbk --algorithm cutMix
