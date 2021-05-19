FROM circleci/python:3.9.4-buster

SHELL ["/bin/bash", "-c"]

# Install most dependencies
RUN sudo apt-get update && sudo apt-get install -y build-essential \
						   wget \
			            		   curl \
			            		   doxygen \
			            		   gcc-multilib \
     					           p7zip-full \ 
						   gcc-arm-none-eabi \
						   binutils-arm-none-eabi \
						   lsb-release \
						   software-properties-common \
			            		   git \
			            		   python3-dev \
			            		   python3-venv

# Install LLVM 10
RUN wget https://apt.llvm.org/llvm.sh -O /home/circleci/llvm.sh
RUN chmod +x /home/circleci/llvm.sh
RUN sudo /home/circleci/llvm.sh 10
RUN sudo apt-get install -y clang-tidy-10
RUN sudo ln -s /usr/lib/llvm-10/bin/clang-tidy /usr/bin/clang-tidy
RUN sudo ln -s /usr/lib/llvm-10/bin/clang /usr/bin/clang

# Install Node 12
RUN curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
RUN sudo apt-get install -y nodejs

# Download CodeChecker Sources
RUN git clone https://github.com/Ericsson/CodeChecker.git --depth 1 /home/circleci/codechecker

# Build CodeChecker
WORKDIR /home/circleci/codechecker
RUN make venv
RUN source $PWD/venv/bin/activate
RUN make package
ENV PATH /home/circleci/codechecker/build/CodeChecker/bin:${PATH}

WORKDIR /home/circleci

CMD ["/bin/bash"]
