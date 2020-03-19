import os
from pathlib import Path
import cppjieba_pybind as cppjieba

root = Path(os.path.dirname(os.path.abspath(__file__)))

DICT_PATH = root / "dict/jieba.dict.utf8"
HMM_PATH = root / "dict/hmm_model.utf8"
USER_DICT_PATH = root / "dict/user.dict.utf8"
IDF_PATH = root / "dict/idf.utf8"
STOP_WORDS_PATH = root / "dict/stop_words.utf8"


class Jieba:

    def __init__(self,
                 dict_path: str = str(DICT_PATH),
                 hmm_path: str = str(HMM_PATH),
                 user_dict_path: str = str(USER_DICT_PATH),
                 idf_path: str = str(IDF_PATH),
                 stop_words_path: str = str(STOP_WORDS_PATH)
                 ):
        self.jb = cppjieba.Jieba(dict_path, hmm_path,
                                 user_dict_path,
                                 idf_path,
                                 stop_words_path)

    def cut(self, text: str, hmm: bool = True) -> list:
        return self.jb.cut(text, hmm)

    def cut_all(self, text: str) -> list:
        return self.jb.cut_all(text)

    def cut_for_search(self, text: str) -> list:
        return self.jb.cut_for_search(text)

    def pseg(self, text: str) -> list:
        return self.jb.pseg(text)

    def extract(self, text: str, topk: int = 5) -> list:
        return self.jb.extract(text, topk)
