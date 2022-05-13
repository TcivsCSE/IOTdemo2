const char index_html[] PROGMEM = R"rawliteral(
<html lang="en"><head>
    <title>彩燈控制</title>
    <meta charset="UTF-8">
    <link rel="icon" href="data:,">
    <link rel="stylesheet" href="./main.css">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        html {font-family: Arial;}
        h2 {
            margin-top: 200px;
            font-size: 3.0rem;
            text-align: center;
            
        }
        :root{
            --width: 70px;
            --height: calc(var(--width) * 0.5);
        } 
        main {
            margin-top: 50px;  
        }
        .switch{
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .switch > input {
          display: none;
        }
        .switch > span::before {        
            position: absolute;
            right: 50%;
            width: calc(var(--height) - 4px);
            height: calc(var(--height) - 4px);
            content: "";
            border: 2px solid #dddddd;
            border-radius: var(--height);
            background-color: #ffffff;
            transition: 0.3s;
        }
        .switch > input:checked + span::before {
            transform: translateX(var(--height));
            border: 2px solid #4cd864;
            transition: 0.3s;
        }
        .switch > span {
            position: relative;
            display: block;
            width: var(--width);
            height: var(--height);
            border-radius: var(--height);
            background-color: #dddddd;
        }
        input:checked + span {
            background-color: #4cd864;
        }
        .box {
            display: flex;
            justify-content: center;
            align-items: center;
            margin: auto;
        }
        .box > .switch > p {
            user-select: none;
            font-size: 24px;
            margin-right: 20px;
        }
        button {
            display: block;
            margin: auto;
            margin-top: 50px;
            padding: 15px 35px;
            color: #0976fc;
            background-color: #f7f7f7;
            border: 0;
            border-radius: 10px;
            font-size: 24px;
        }
        .modelist-div {
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 24px;
        }
        .modelist-div > p {
            margin-right: 20px;
        }
        #modelist {
            font-size: 15px;
            padding: 5px 5px;
            padding-right: 50px;
        }
    </style>
</head>
<body>
    <h2>ESP Web Server</h2>
    <main>
        <div class="modelist-div">
            <p>彩燈模式選擇</p>
            <select id="modelist">
                <option value="">請選擇</option>
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
                <option value="5">5</option>
                <option value="6">6</option>
            </select>
        </div>
        <div class="box">
            <label class="switch">
                <p>彩燈開關</p>
                <input type="checkbox" id="5">
                <span></span>
            </label>
        </div>
    </main>
    <script>
        let checkboxs = document.querySelectorAll('input[type=checkbox]');
        let select = document.querySelector("#modelist");
        select.addEventListener("change", (ev) => {
            console.log(ev.target.value);
            fetch(`/update?state=${ev.target.value}`)
            .catch(error => {})
        });
        checkboxs.forEach(checkbox => {
            let flag = 1;
            checkbox.addEventListener("change", () => {
              console.log(1)
                if(checkbox.checked){ 
                    let selectlists = document.getElementById("modelist");
                    let ind = selectlists.selectedIndex;
                    let list_val = selectlists.options[ind].value;
                    let tex = selectlists.options[ind].text;
                    fetch(`/update?state=${list_val}`)
                    .catch(error => {})
                    checkbox.disabled = true;
                    setTimeout(() => {
                        checkbox.disabled = false;
                        //fetch(`/update?output=${checkbox.id}&state=0`)
                        //.catch(error => {})
                    }, 500);
                }
                else{
                  fetch(`/update?output=${checkbox.id}&state=0`)
                    .catch(error => {})
                }
            })
        })
        function logoutButton() {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/logout", true);
          xhr.send();
          setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
        }
    </script>

</body></html>)rawliteral";
