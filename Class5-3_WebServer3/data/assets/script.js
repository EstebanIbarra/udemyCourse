/**
 * La primera sección de este código es mi refactorización del código de la clase
 * En esta versión la consola arroja error cuando el ESP32 no alcanza a retornar la respuesta en tiempo
 */

const getMessage = async () => {
    let message = document.getElementById('message');
    let serverMessage = await fetch('/message');
    serverMessage.ok ? message.innerText = await serverMessage.text() : alert('HTTP-Error: ' + serverMessage.status);
}

const getData = () => {
    let adcLecture = document.getElementById('adc');
    setInterval(
        async function () {
            let adcLectureResponse = await fetch('/adc');
            adcLectureResponse.ok ? adcLecture.innerText = await adcLectureResponse.text() : console.log('HTTP-Error: ' + adcLectureResponse.status);
        },
        1000
    );
}

getData();

/**
 * Este es el código original de la clase, la diferencia es la implementación del método fetch vs crear la instancia de XMLHttpRequest
 * En esta versión el ESP32 rebootea cada X tiempo, causa desconocida
 */

/*
function getMessage() {
    // Elaboración de la petición al servidor
    let xhttp = new XMLHttpRequest();
    xhttp.open('GET', '/message', true);
    xhttp.send();
    // Acción a realizar en cuanto se obtenga la respuesta del servidor
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById('message').innerHTML = this.responseText;
        }
    }
}

setInterval(
    () => {
        let xhttp = new XMLHttpRequest();
        xhttp.open('GET', '/adc', true);
        xhttp.send();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById('adc').innerHTML = this.responseText;
            }
        }
    },
    300
)
*/