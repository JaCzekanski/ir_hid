<script>
	import HidConsumer from './hid_consumer.js';
	import HidKeyboard from './hid_keyboard.js';

	let webusbSupported = navigator.usb != undefined
	let device = null
	let deviceName = "ir-hid"
	let listenerInterval = null
	let lastPressedKey = null /*{
		protocol: 0,
		address: 0,
		command: 0
	}*/
	let configuredKeys = null
	let listenForLastPressedKey = false

	let scancode = null
	let keyName = ""

	let mediaKeySelected = null

	const protocols = [
		"UNKNOWN",
		"SIRCS",
		"NEC",
		"SAMSUNG",
		"MATSUSHITA",
		"KASEIKYO",
		"RECS80",
		"RC5",
		"DENON",
		"RC6",
		"SAMSUNG32",
		"APPLE",
		"RECS80EXT",
		"NUBERT",
		"BANG_OLUFSEN",
		"GRUNDIG",
		"NOKIA",
		"SIEMENS",
		"FDC",
		"RCCAR",
		"JVC",
		"RC6A",
		"NIKON",
		"RUWIDO",
		"IR60",
		"KATHREIN",
		"NETBOX",
		"NEC16",
		"NEC42",
		"LEGO",
		"THOMSON",
		"BOSE",
		"A1TVBOX",
		"ORTEK",
		"TELEFUNKEN",
		"ROOMBA",
		"RCMM32",
		"RCMM24",
		"RCMM12",
		"SPEAKER",
		"LGAIR",
		"SAMSUNG48",
		"MERLIN",
		"PENTAX",
		"FAN",
		"S100",
		"ACP24",
		"TECHNICS",
		"PANASONIC",
		"MITSU_HEAVY",
		"VINCENT",
		"SAMSUNGAH",
		"IRMP16",
		"GREE",
		"RCII",
		"METZ",
		"ONKYO"
	];

	if (webusbSupported) {
		// navigator.usb.onconnect = () => {
		// 	console.log("onconnect")
		// 	if (device == null) connectToDevice()
		// }
		navigator.usb.ondisconnect = () => {
			console.log("ondisconnect")
			if (device && !device.opened) disconnect()
		}
	}

	async function connectToDevice() {
		const filters = [{
			vendorId: 0x16c0,
			productId: 0x03e8
		}]
		try {
			device = await navigator.usb.requestDevice({ filters: filters });
		}
		catch( e) {
			console.error(e)
			return
		}

      	await device.open()
		await device.selectConfiguration(1)
	}

	const REQUEST_READ_LAST = 1
	const REQUEST_ENTRY_COUNT = 2
	const REQUEST_READ_ENTRY = 3
	const REQUEST_WRITE_ENTRY = 4

	async function listenForPressedKey() {
		let result = await device.controlTransferIn({
			requestType: 'vendor',
			recipient: 'device',
			request: REQUEST_READ_LAST,
			value: 0x00,
			index: 0x00
		}, 8);

		if (result.status != "ok") {
			return
		}
		const protocol = result.data.getUint8(0)
		if (protocol == 0) {
			return
		}

		lastPressedKey = {
			protocol: protocol,
			address: result.data.getUint8(1) | (result.data.getUint8(2) << 8),
			command: result.data.getUint8(3) | (result.data.getUint8(4) << 8)
		}
	}

	function disconnect() {
		if (listenerInterval != null) {
			clearInterval(listenerInterval)
			listenerInterval = null
		}

		device.close();
		device = null
	}

	$: if (listenForLastPressedKey) {
		listenerInterval = setInterval(listenForPressedKey, 500)
	} else {
		if (listenerInterval != null) {
			clearInterval(listenerInterval)
			listenerInterval = null
		}
	}

$: console.log("mediaKeySelected "+ mediaKeySelected)
	function hex(e) {
		return "0x" + e.toString(16).padStart(4, '0')
	}

	function removeBinding(binding) {
		configuredKeys = configuredKeys.filter((it) => {
			it !== binding
		})
	}

	async function addKeyboardBinding() {
		// Create buffer array
		let buffer = new ArrayBuffer(8)
		let view = new Uint8Array(buffer);

		let e = lastPressedKey		
		view[0] = e.protocol
		view[1] = e.address & 0xff
		view[2] = (e.address>>8) & 0xff
		view[3] = e.command & 0xff
		view[4] = (e.command>>8) & 0xff
		view[5] = 1 // reportId
		view[6] = scancode & 0xff;
		view[7] = (scancode>>8) & 0xff;

		let result = await device.controlTransferOut({
			requestType: 'vendor',
			recipient: 'device',
			request: REQUEST_WRITE_ENTRY,
			value: 0x00,
			index: 0x00
		}, buffer);

		if (result.status != "ok") {
			console.log("addBinding " + result.status);
			return
		}

		lastPressedKey = null
		scancode = null
		keyName = ""

		mediaKeySelected = null
	}

	async function addMediaBinding() {
		// Create buffer array
		let buffer = new ArrayBuffer(8)
		let view = new Uint8Array(buffer);

		let e = lastPressedKey		
		view[0] = e.protocol
		view[1] = e.address & 0xff
		view[2] = (e.address>>8) & 0xff
		view[3] = e.command & 0xff
		view[4] = (e.command>>8) & 0xff
		view[5] = 2 // reportId
		view[6] = mediaKeySelected & 0xff;
		view[7] = (mediaKeySelected>>8) & 0xff;

		let result = await device.controlTransferOut({
			requestType: 'vendor',
			recipient: 'device',
			request: REQUEST_WRITE_ENTRY,
			value: 0x00,
			index: 0x00
		}, buffer);

		if (result.status != "ok") {
			console.log("addBinding " + result.status);
			return
		}

		lastPressedKey = null
		scancode = null
		keyName = ""

		mediaKeySelected = null
	}

	function getScancode(e) {
		scancode = e.keyCode.toString(10)
		keyName = e.key
	}


	async function readButtons() {
		let result = await device.controlTransferIn({
			requestType: 'vendor',
			recipient: 'device',
			request: REQUEST_ENTRY_COUNT,
			value: 0x00,
			index: 0x00
		}, 1);

		if (result.status != "ok") {
			console.log("REQUEST_ENTRY_COUNT " + result.status);
			return
		}

		const count = result.data.getUint8(0);
		console.log("entry count: " + count)

		configuredKeys = []
		for (let i = 0; i< count; i++) {
			let result;
			for (;;) {
				await new Promise(r => setTimeout(r, 200));
				result = await device.controlTransferIn({
					requestType: 'vendor',
					recipient: 'device',
					request: REQUEST_READ_ENTRY,
					value: 0x00,
					index: i
				}, 8);

				if (result.status != "ok") {
					console.log("REQUEST_READ_ENTRY " + result.status);
					continue;
				}
				break;
			}
			
			const protocol = result.data.getUint8(0)
			if (protocol == 0 || protocol == 0xff) {
				break;
			}

			const e = {
				protocol: protocol,
				address: result.data.getUint8(1) | (result.data.getUint8(2) << 8),
				command: result.data.getUint8(3) | (result.data.getUint8(4) << 8),
				reportId: result.data.getUint8(5),
				button: result.data.getUint8(6) | (result.data.getUint8(7) << 7),
			}
		
			configuredKeys = [...configuredKeys, e]
		}
	}
</script>

<main>
	<h1>ir-hid</h1>

	{#if !webusbSupported}
		<p>WebUSB is not supported, please use a modern browser.</p>
	{:else if !device}
		<button on:click={connectToDevice}>Connect</button>
	{:else}
		<button on:click={disconnect}>Disconnect</button>
		<span>Connected to {device.productName} ({device.manufacturerName}) </span>

		<br>
		<label><input type="checkbox" bind:checked={listenForLastPressedKey} /> Listen for remote</label>
		<div class="lastPressedKey">
		{#if lastPressedKey}
			<p>Last pressed key:</p>
			<div>Protocol: {protocols[lastPressedKey.protocol]}</div>
			<div>Address: {hex(lastPressedKey.address)}</div>
			<div>Command: {hex(lastPressedKey.command)}</div>

			<label>Keyboard 
				<input on:keydown|preventDefault={getScancode} value={scancode + ` (${keyName})`} type="text"/>
				<select bind:value={scancode}>
					{#each HidKeyboard as e}
						<option value={e.id}>{e.name}</option>
					{/each}
				</select>
				<button on:click|preventDefault={addKeyboardBinding} disabled={scancode==null}>Add keyboard binding</button>
			</label>
			

			<label>Media 
				<select bind:value={mediaKeySelected}>
					{#each HidConsumer as e}
						<option value={e.id}>{e.name}</option>
					{/each}
				</select>
				<button on:click|preventDefault={addMediaBinding} disabled={mediaKeySelected==null}>Add media binding</button>
			</label>

	
		{/if}
		</div>

		<br>
		{#if configuredKeys}
			Configured keys:
			{#each configuredKeys as e}
				<div>
					<button class="removeBinding" on:click={removeBinding(e)}>X</button>
					{e.button} ({protocols[e.protocol]} {hex(e.address)} {hex(e.command)})
				</div>
			{/each}
		{/if}

		<br>
		<button on:click={readButtons}>Refresh list</button>
	{/if}

</main>

<style>
	main {
		text-align: left;
		padding: 1em;
		max-width: 240px;
		margin: 0 auto;
	}

	h1 {
		color: #ff3e00;
		text-transform: uppercase;
		font-size: 3em;
		font-weight: 100;
	}

	@media (min-width: 640px) {
		main {
			max-width: none;
		}
	}

	button.removeBinding {
		color: #ff3e00;
		font-family: monospace;
		height: 2rem;
		width: 2rem;
		font-size: 1rem;
	}

	.lastPressedKey {
		height: 200px;
	}
</style>