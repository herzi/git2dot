#!/bin/bash
# This file is part of herzi's playground
#
# AUTHORS
#     Sven Herzberg  <herzi@gnome-de.org>
#
# Copyright (C) 2008  Sven Herzberg
#
# This work is provided "as is"; redistribution and modification
# in whole or in part, in any medium, physical or electronic is
# permitted without restriction.
#
# This work is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# In no event shall the authors or contributors be liable for any
# direct, indirect, incidental, special, exemplary, or consequential
# damages (including, but not limited to, procurement of substitute
# goods or services; loss of use, data, or profits; or business
# interruption) however caused and on any theory of liability, whether
# in contract, strict liability, or tort (including negligence or
# otherwise) arising in any way out of the use of this software, even
# if advised of the possibility of such damage.

tmpfile=`mktemp`

./git2dot | dot -Tpng -o"$tmpfile"
eog "$tmpfile"

