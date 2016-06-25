# Copyright 1999-2010 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=6

inherit git-r3

EGIT_REPO_URI="https://github.com/Triiuark/plow"

DESCRIPTION="command-line playlist generator"
HOMEPAGE="http://projekt182.de/en/software/plow.html"
SRC_URI=""

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64 ~x86"

DEPEND=">=media-libs/taglib-1.4
	>=dev-db/sqlite-3.6
	dev-util/pkgconfig"
RDEPEND=">=media-libs/taglib-1.4
	>=dev-db/sqlite-3.6"

src_install() {
	exeinto /usr/bin
	doexe ${S}/bin/plow
	doman ${S}/plow.1
}

