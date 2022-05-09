const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <title>3F選手室門禁系統</title>
    <meta charset="UTF-8">
    <link rel="icon" href="data:,">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
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
    </style>
</head>
<body>
    <h2>ESP Web Server</h2>
    <main>
      %BUTTONPLACEHOLDER%  
      <button onclick="logoutButton()">Logout</button>
    </main>
    <script>
        let checkboxs = document.querySelectorAll('input[type=checkbox]');
        checkboxs.forEach(checkbox => {
            let flag = 1;
            checkbox.addEventListener("change", () => {
              console.log(1)
                if(checkbox.checked){ 
                    fetch(`/update?output=${checkbox.id}&state=1`)
                    .catch(error => {})
                    checkbox.disabled = true;
                    setTimeout(() => {
                        checkbox.checked = false;
                        checkbox.disabled = false;
                        fetch(`/update?output=${checkbox.id}&state=0`)
                        .catch(error => {})
                    }, 500);
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
</body>
</html>)rawliteral";
