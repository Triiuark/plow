# Copyright 1999-2016 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Id$

EAPI=6

DESCRIPTION="Plow is a command-line music library and playlist creation tool."
HOMEPAGE="https://github.com/Triiuark/plow"
SRC_URI="https://github.com/Triiuark/plow/releases/download/v0.6.0/plow-0.6.0.tar.gz"

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

