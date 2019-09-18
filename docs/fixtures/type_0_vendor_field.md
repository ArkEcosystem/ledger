
# Type 0: Transfer with VendorField

## Signed Json Payload

```javascript
{
	"version": 2,
	"network": 23,
	"type": 0,
	"nonce": "2",
	"senderPublicKey": "03d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c",
	"fee": "10000000",
	"amount": "1",
	"vendorFieldHex": "302e3132373131363230373030383934323434",
	"vendorField": "0.12711620700894244",
	"expiration": 0,
	"recipientId": "AUbLwNEQWMshw4vBGYyR8JWn4Lx6sJbj6M",
	"signature": "504215bf61f7e8e0d4cd7c7e1511b501367e8c2f3543972906a3b80d42cebc3e4ec974f938124661cb65eab93dacba6ba0f5045861ac28fc0287462557ffd99b",
	"id": "8bd461006dc6481c17b38f652e775d151fb36e8b3f390fb213b6b5c399df6c97"
}
```

## Serialized Payload

```shell
ff0217010000000000020000000000000003d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c809698000000000013302e3132373131363230373030383934323434010000000000000000000000178c9bd74222025a19063c8fca8a50c39a891feeca504215bf61f7e8e0d4cd7c7e1511b501367e8c2f3543972906a3b80d42cebc3e4ec974f938124661cb65eab93dacba6ba0f5045861ac28fc0287462557ffd99b
```

## Deserializing the Hex

| Key                       | Pos.      | Size<br>_(bytes)_ | Value<br> _(hex)_     |
| :--                       | :--:      | :---------------: | :----------------     |
| **Header:**               | **[0]**   | **1**             | `0xff`                |
| **Version:**              | **[1]**   | **1**             | `0x02`                |
| **Network:**              | **[2]**   | **1**             | `0x17`                |
| **Typegroup:**            | **[3]**   | **4**             | `0x01000000`          |
| **Type:**                 | **[7]**   | **2**             | `0x0000`              |
| **Nonce:**                | **[9]**   | **8**             | `0x0200000000000000`  |
| **SenderPublicKey:**      | **[17]**  | **33**            | `0x03d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c`    |
| **Fee:**                  | **[50]**  | **8**             | `0x8096980000000000`  |
| **VendorField Length:**   | **[58]**  | **1**             | `0x13`                |
| **VendorField:**          | **[59]**  | **19**            | `0x302e3132373131363230373030383934323434`    |
| **Amount:**               | **[78]**  | **8**             | `0x0100000000000000`  |
| **Expiration:**           | **[86]**  | **4**             | `0x00000000`          |
| **Recipient:**            | **[90]**  | **21**            | `0x178c9bd74222025a19063c8fca8a50c39a891feeca`    |
| **Signature:**            | **[111]** | **64**            | `0x504215bf61f7e8e0d4cd7c7e1511b501367e8c2f3543972906a3b80d42cebc3e4ec974f938124661cb65eab93dacba6ba0f5045861ac28fc0287462557ffd99b`  |

## Example Ledger Payload

```shell
python examples/example_helper.py --tx ff0217010000000000020000000000000003d59f3b7d698536f6925a77f22d484d518b06a2c09318e8e5ff487afcdedefb2c809698000000000013302e3132373131363230373030383934323434010000000000000000000000178c9bd74222025a19063c8fca8a50c39a891feeca
```
> _note that signature bytes are removed (i.e. there's no need to sign a signed-transaction)._
