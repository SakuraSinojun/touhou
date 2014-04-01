#! /bin/bash

usage () {
    echo "$0 pk8key pemkey outputkeystore keyname password"
}


PK8KEY=$1
PEMKEY=$2
KEYSTO=$3
KEYNAM=$4
PASSWD=$5

if [ "$#" != "5" ]; then
    usage $0
    exit 1
fi

openssl pkcs8 -in $PK8KEY -inform DER -outform PEM -out shared.priv.pem -nocrypt
openssl pkcs12 -export -in $PEMKEY -inkey shared.priv.pem -out shared.pk12 -name $KEYNAM << EOF
$PASSWD
$PASSWD
EOF

keytool -importkeystore -deststorepass $PASSWD -destkeypass $PASSWD -destkeystore $KEYSTO -srckeystore shared.pk12 -srcstoretype PKCS12 -srcstorepass $PASSWD -alias $KEYNAM
rm shared.priv.pem
rm shared.pk12


