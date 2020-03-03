Vagrant.configure("2") do |config|

    # Specify the base box
    config.vm.box = "ubuntu/bionic64"
    config.vm.box_version = "20190814.0.0"

    # set the `app`, `examples`, `glyphs`, and `src` folders for syncing
    config.vm.synced_folder("apps/", "/home/vagrant/apps/",
                            id: "appsdir",
                            :create => true,
                            type: "virtualbox")
    config.vm.synced_folder("examples/", "/home/vagrant/apps/ledger-app-ark/examples/",
                            id: "examplesdir",
                            :create => true,
                            type: "virtualbox")
    config.vm.synced_folder("glyphs/", "/home/vagrant/apps/ledger-app-ark/glyphs/",
                            id: "glyphsdir",
                            :create => true,
                            type: "virtualbox")
    config.vm.synced_folder("src/", "/home/vagrant/apps/ledger-app-ark/src/",
                            id: "srcdir",
                            :create => true,
                            type: "virtualbox")

    # copy the scripts directory and app Makefile to vagrant machine
    config.vm.synced_folder("scripts/", "/home/vagrant/apps/ledger-app-ark/scripts/",
                            id: "scriptsdir",
                            :create => true,
                            type: "virtualbox")
    config.vm.provision("file",
                        source: "./Makefile",
                        destination: "/home/vagrant/apps/ledger-app-ark/")

    # Copy Icons
    config.vm.provision("file",
                        source: "./icons/nanos_app_ark.gif",
                        destination: "/home/vagrant/apps/ledger-app-ark/icons/")
    config.vm.provision("file",
                        source: "./icons/nanox_app_ark.gif",
                        destination: "/home/vagrant/apps/ledger-app-ark/icons/")

    # VM specific configs
    config.vm.provider "virtualbox" do |v|
        v.name = "ARK Ledger App Development Box"
        v.customize ["modifyvm", :id, "--memory", "1024"]

        # Connect Ledger Nano S throug usb
        v.customize ["modifyvm", :id, "--usb", "on"]
        v.customize ["modifyvm", :id, "--usbehci", "on"]
        v.customize ["usbfilter", "add", "0",
                     "--target", :id,
                     "--name", "Ledger Nano S",
                     "--manufacturer", "Ledger",
                     "--product", "Nano S"]
    end

    # Shell provisioning
    config.vm.provision "shell" do |s|
        s.path = "scripts/provision.sh"
    end
end
