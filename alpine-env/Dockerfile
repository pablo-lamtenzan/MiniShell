FROM alpine

ENV REPO="https://github.com/pablo-lamtenzan/minishell.git"
ENV NAME="minishell"
ENV CC="gcc"
ENV CFLAGS="-Wall -Werror"

ENV BUILD_DIR="/build"

RUN apk add --update --no-cache build-base git ncurses-dev

COPY srcs/entrypoint.sh /entrypoint.sh

ENTRYPOINT ["sh", "/entrypoint.sh"]

CMD ["$BUILD_DIR/$NAME"]