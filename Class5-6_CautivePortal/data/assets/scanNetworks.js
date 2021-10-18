const networkSelector = document.getElementById('networks');
const passwordInput = document.getElementById('password');
const getNetworks = async () => {
    let networks = await fetch('/search');
    networks.ok ? appendSSID(networkSelector, await networks.text()) : alert('HTTP-Error' + networks.status);
}

const hideInput = () => {
    let selectedNetwork = networkSelector.children[networkSelector.selectedIndex];
    selectedNetwork.classList.contains('open') ? passwordInput.classList.add('d-none') : passwordInput.classList.remove('d-none');
}

const appendSSID = (selector, networks) => {
    networks = JSON.parse(networks).Networks;
    networks.map(network => {
        let option = document.createElement('option');
        let networkType = network.networkType === 'Open Network' ? 'open' : 'closed';
        option.value = network.SSID;
        option.innerText = network.SSID + ' - ' + network.networkType;
        option.classList.add(networkType);
        selector.appendChild(option);
    });
    selector.selectedIndex = '0';
}

getNetworks();