.. only:: not (epub or latex or html)

    WARNING: You are looking at unreleased Cilium documentation.
    Please use the official rendered version released here:
    https://docs.cilium.io

.. _kata:

***************************
Kata Containers with Cilium
***************************

`Kata Containers <https://katacontainers.io/>`_ is an open source project that
provides a secure container runtime with lightweight virtual machines that feel
and perform like containers, but provide stronger workload isolation using
hardware virtualization technology as a second layer of defense.
Similar to the OCI runtime ``runc`` provided by Docker, Cilium can be used with
Kata Containers, providing a higher degree of security at the network layer and
at the compute layer with Kata.

This guide provides a walkthrough of installing Cilium on Kata Containers. It
assumes that you have already followed the official
`Kata Containers installation user guide <https://github.com/kata-containers/documentation/tree/master/install>`_
to get the Kata Containers runtime up and running on your platform of choice
but that you haven't yet setup Kubernetes.

.. note::
   This guide has been validated by following the Kata Containers guide for
   Google Compute Engine (GCE) and using Ubuntu 18.04 LTS with the packaged
   version of Kata Containers, CRI-containerd and Kubernetes 1.18.3.

Setup Kubernetes with CRI
=========================

Kata Containers runtime is an OCI compatible runtime and cannot directly
interact with the CRI API level. For this reason, it relies on a CRI
implementation to translate CRI into OCI. At the time of writing this guide,
there are two supported ways called CRI-O and CRI-containerd. It is up to you
to choose the one that you want, but you have to pick one.

Refer to the section :ref:`k8s_requirements` for detailed instruction on how to
prepare your Kubernetes environment and make sure to use Kubernetes >= 1.12.
Then, follow the
`official guide to run Kata Containers with Kubernetes <https://github.com/kata-containers/documentation/blob/master/how-to/run-kata-with-k8s.md>`_.

.. note::
   Minimum version of kubernetes 1.12 is required to use the RuntimeClass Feature
   for Kata Container runtime described below.

With your Kubernetes cluster ready, you can now proceed to deploy Cilium.

Deploy Cilium
=============

.. include:: k8s-install-download-release.rst

Deploy Cilium release via Helm:

  .. tabs::

     .. group-tab:: Using CRI-O

        .. parsed-literal::

           helm install cilium |CHART_RELEASE| \\
             --namespace kube-system \\
             --set global.containerRuntime.integration=crio

     .. group-tab:: Using CRI-containerd

        .. parsed-literal::

           helm install cilium |CHART_RELEASE| \\
             --namespace kube-system \\
             --set global.containerRuntime.integration=containerd

.. include:: k8s-install-validate.rst

Install Kata on a running Kubernetes Cluster
============================================

Kubernetes configured with CRI runtimes by default uses ``runc`` runtime for
running a workload. You will need to configure Kubernetes to be able to use an
alternate runtime.

`RuntimeClass <https://kubernetes.io/docs/concepts/containers/runtime-class/>`_
is a Kubernetes feature first introduced in Kubernetes 1.12 as alpha. It is the
feature for selecting the container runtime configuration to use to run a pod’s
containers.

To use Kata-Containers, ensure the RuntimeClass feature gate is enabled for k8s
< 1.13. It is enabled by default on k8s 1.14. See
`Feature Gates <https://kubernetes.io/docs/reference/command-line-tools-reference/feature-gates/>`_
for an explanation of enabling feature gates.

To install Kata Containers and configure CRI to use Kata as a one step process,
you will use `kata-deploy <https://github.com/kata-containers/packaging/tree/master/kata-deploy>`_
and follow the official instructions to
`install Kata on a running k8s cluster <https://github.com/kata-containers/packaging/tree/master/kata-deploy#kubernetes-quick-start>`_.

Run Kata Containers with Cilium CNI
===================================

Now that Kata is installed on the k8s cluster, you can run a sample workload
with Kata Containers with Cilium as the CNI by following
`these instructions <https://github.com/kata-containers/packaging/tree/master/kata-deploy#run-a-sample-workload>`_.
