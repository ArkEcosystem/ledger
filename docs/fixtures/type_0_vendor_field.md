
# Type 0: Transfer with VendorField

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 0,
	"nonce": "3",
	"senderPublicKey": "029f0ab8ab10258144332230178353fa24eb4274370345eaaf1594948a79c89399",
	"fee": "10000000",
	"amount": "1",
	"vendorFieldHex": "48656c6c6f20576f726c64",
	"vendorField": "Hello World",
	"expiration": 0,
	"recipientId": "AMLFBUUXs8ss9iq4k5fLLnZJ5TV757dUFV",
	"signature": "0e23e11473c80ea9e2d32d32bb338a649ad09ac1e4b302ec938efcbe3046176344495e538d3fb1b95efd1f2d0d847b34eee0c440ae4575446d93e5c4219c0e63",
	"id": "ffc4ace894fb29d986d0b11829e67740b47056bd362a52a5eaeda8cf8c18d349"
}
```

## Serialized Payload

```shell
ff02170100000000000300000000000000029f0ab8ab10258144332230178353fa24eb4274370345eaaf1594948a79c8939980969800000000000b48656c6c6f20576f726c64010000000000000000000000173cf7ea59e8d9690e858b7674885b9a4a2c4365d60e23e11473c80ea9e2d32d32bb338a649ad09ac1e4b302ec938efcbe3046176344495e538d3fb1b95efd1f2d0d847b34eee0c440ae4575446d93e5c4219c0e63
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0000`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0300000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x029f0ab8ab10258144332230178353fa24eb4274370345eaaf1594948a79c89399`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x0b`                |
| **VendorField:**          | **[59]**  | **11**            | `0x48656c6c6f20576f726c64`    |
| **Amount:**               | **[70]**  | **8**             | `0x0100000000000000`  |
| **Expiration:**           | **[78]**  | **4**             | `0x00000000`          |
| **Recipient:**            | **[82]**  | **21**            | `0x173cf7ea59e8d9690e858b7674885b9a4a2c4365d6`    |
| **Signature:**            | **[103]** | **64**            | `0x0e23e11473c80ea9e2d32d32bb338a649ad09ac1e4b302ec938efcbe3046176344495e538d3fb1b95efd1f2d0d847b34eee0c440ae4575446d93e5c4219c0e63`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff02170100000000000300000000000000029f0ab8ab10258144332230178353fa24eb4274370345eaaf1594948a79c8939980969800000000000b48656c6c6f20576f726c64010000000000000000000000173cf7ea59e8d9690e858b7674885b9a4a2c4365d6
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
