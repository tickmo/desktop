#!/usr/bin/env bash

# apt-get update
apt-get install -y vim git nginx supervisor
if ! [ -L /var/www ]; then
  rm -rf /var/www
  ln -fs /vagrant /var/www
fi
cp /vagrant/vagrant/nginx.conf /etc/nginx/sites-enabled/tyrion

apt-get install -y supervisor
cp /vagrant/vagrant/supervisor.conf /etc/supervisor/conf.d/tyrion.conf

cd /usr/local/share
git clone -b master git://github.com/nim-lang/Nim.git nim
cd nim
git clone -b master --depth 1 git://github.com/nim-lang/csources csources
cd csources && sh build.sh && cd .. && bin/nim c koch && ./koch boot -d:release
chmod a+x ./bin/nim
ln -s /usr/local/share/nim/bin/nim /usr/bin/nim
cd ..
git clone https://github.com/nim-lang/nimble.git nimble
cd nimble
nim c -r src/nimble install
chmod a+x ./nimble
ln -s /usr/local/share/nimble/nimble /usr/bin/nimble

echo 'export PATH="$PATH:$HOME/.nimble/bin"' >> /home/vagrant/.bashrc
cd /var/www
sudo -u vagrant nimble build -y

service nginx restart
service supervisor restart
