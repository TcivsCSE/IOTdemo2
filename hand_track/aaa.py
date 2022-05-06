import wget
import requests
import json
session_requests = requests.session()
result = session_requests.get("http://www.chwa.com.tw/newCiv/chwa108/asp/getBookResources.asp?BookNo=04F27106")
print(result.text[1])
aa = json.loads(result.text)["mistake"]
print(aa)

#使用GET方法取得網頁資訊
i=0
while True:
    try:
        while i<len(aa):
            print(aa[i]["name"] + '_' + aa[i]["explain"])
            print(aa[i]["link"])
            aurl = aa[i]["link"]
            result = session_requests.get(aurl)
            print(result.status_code, result.url)
            read_file_name = (aa[i]["name"] + '_' + aa[i]["explain"]).replace("/","_") + result.url[-4::]
            print(read_file_name)
            if aa[i]["kind"]!='15':
                file_name = wget.download(result.url, out=read_file_name)
            i+=1
    except:
        print("Fail ====")
        print(aa[i]["name"] + '_' + aa[i]["explain"])
        print(aa[i]["link"])
        print("Fail ====")
        i+=1