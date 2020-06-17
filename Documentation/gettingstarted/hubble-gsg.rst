.. only:: not (epub or latex or html)

    WARNING: You are looking at unreleased Cilium documentation.
    Please use the official rendered version released here:
    https://docs.cilium.io

.. _hubble_gsg:

***************************
Getting Started with Hubble
***************************

set up a cluster using kind and demonstrate how to use hubble.

.. include:: kind-install.rst

Deploy Cilium and Hubble
========================

.. include:: k8s-install-download-release.rst

**(optional, but recommended)** Pre-load Cilium images into the kind cluster so each worker doesn't have to pull them.

.. parsed-literal::

  docker pull cilium/cilium:|IMAGE_TAG|
  kind load docker-image cilium/cilium:|IMAGE_TAG|

Install Cilium release via Helm:

.. parsed-literal::

   helm install cilium |CHART_RELEASE| \\
      --namespace kube-system \\
      --set global.nodeinit.enabled=true \\
      --set global.kubeProxyReplacement=partial \\
      --set global.hostServices.enabled=false \\
      --set global.externalIPs.enabled=true \\
      --set global.nodePort.enabled=true \\
      --set global.hostPort.enabled=true \\
      --set global.pullPolicy=IfNotPresent \\
      --set global.hubble.enabled=true \\
      --set global.hubble.listenAddress=":4244" \\
      --set global.hubble.relay.enabled=true \\
      --set global.hubble.ui.enabled=true

Install Hubble CLI
==================
.. include:: hubble-install.rst

Port Forward
============

.. parsed-literal::

   kubectl port-forward -n kube-system svc/hubble-relay 4245:80
   kubectl port-forward -n kube-system svc/hubble-ui 12000:80

.. parsed-literal::
   hubble observe --last 1 -o json --debug --server localhost:4245

Open http://localhost:12000/ from a browser.
