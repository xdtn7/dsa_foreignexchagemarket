/*-------------------------------------------------
PROJECT: FOREIGN EXCHANGE MARKET
NAME: NGUYEN DUY TINH
Student ID: 1852797
Data Structure and Algorithms - Computer Enginering
Ho Chi Minh University of Technology
--------------------------------------------------*/

#include "processData.h"

enum CodeValue { insCode, delCode, updCode, stCode, mbCode, djCode, egCode, comCode };

static map<string, CodeValue> s_mapCodeValues = {
	{ "INS", insCode },
	{ "DEL", delCode },
	{ "UPD", updCode },
	{ "ST", stCode },
	{ "MB", mbCode },
	{ "DJ", djCode },
	{ "EG", egCode }
};

ProcessData::Exchange::Candle::Candle() {
	TIME = 0;
	OP = HP = LP = CP = 0;
}
ProcessData::Exchange::Candle::~Candle() {
}

ProcessData::Exchange::Exchange() {
	BC = QC = " ";
	// can = nullptr;
	// can = new LList<Candle>();
}

ProcessData::Exchange::~Exchange() {
	//delete can;
}

ProcessData::ProcessData() {
	// data = nullptr;

	// data->insert0()
}
ProcessData::~ProcessData() {
	for (data->moveToStart(); !data->isLast(); data->next()) {
		data->getValue().can->moveToStart();
		while(!data->getValue().can->isLast())
		data->getValue().can->remove();
		delete data->getValue().can;
	}
	delete data;
}
//----------------------------------------------------
int ProcessData::split(string line, string*& sp) {
	sp = new string[MAXSIZECODE];
	const string SEP = " ";
	int pos, lastpos = 0, idx = 0;
	while (lastpos < line.size()) {
		if (idx == 8) return -1;
		pos = line.find(SEP, lastpos);
		if (pos == line.size() - 1) return -1;
		if (pos < 0) {

			sp[idx] = line.substr(lastpos, line.size() - lastpos);
			return ++idx;
		}
		sp[idx] = line.substr(lastpos, pos - lastpos);
		if (sp[idx] == " ") return -1;
		idx++;
		lastpos = pos + 1;

	}
	return idx;
}

int ProcessData::process(string line) {
	string* p;
	int n = ProcessData::split(line, p);
	if (n <= 0) {
		delete[] p;
		return -1;
	}
	int res = -1;

	/*if (n == 3 && (p[0] == " " || p[1] == " " || p[2] == " ")) return -1;
	if (n == 4 && (p[0] == " " || p[1] == " " || p[2] == " " || p[3] == " ")) return -1;
	if (n == 5 && (p[0] == " " || p[1] == " " || p[2] == " " || p[3] == " " || p[4] == " ")) return -1;
	if (n == 6 && (p[0] == " " || p[1] == " " || p[2] == " " || p[3] == " " || p[4] == " " || p[5] == " " )) return -1;*/

	try {
		switch (s_mapCodeValues[p[0]]) {
		case insCode:
			res = insert(p, n);
			break;
		case updCode:
			// TO DO
			res = up(p, n);
			break;
		case delCode:
			// TO DO
			res = del(p, n);
			break;
		case mbCode:
			// TO DO
			res = mb(p, n);
			break;
		case stCode:
			// TO DO
			res = st(p, n);
			break;
		case djCode:
			// TO DO
			res = dj(p, n);
			break;
		case egCode:
			// TO DO
			res = eg(p, n);
			break;
		default:
			res = -1;
		}
	}
	catch (invalid_argument iax) {
		delete[] p;
		return res;
	}
	delete[] p;
	return res;
}
//-----------------------------------------------------


int ProcessData::insert(const string* sp, const int n) {

	//if (n != 8) return -1;
	if (sp[4] > sp[5] || sp[4] < sp[6] || sp[7] > sp[5] || sp[7] < sp[6]) return -1;

	//BAN DAU KHONG CO EX NAO HET
	if (data == nullptr) {
		data = new LList<Exchange>();	//TAO HEAD NODE CUA EX
		data->insert0();				//INSERT EX DAU TIEN
		data->getValue().BC = sp[1];
		data->getValue().QC = sp[2];
		data->getValue().can = new LList<Exchange::Candle>(); //TAO HEAD NODE CUA CA
		data->getValue().can->insert0();					//INSERT CA DAU TIEN
		data->getValue().can->getValue().TIME = stoi(sp[3]);
		data->getValue().can->getValue().OP = stof(sp[4]);
		data->getValue().can->getValue().HP = stof(sp[5]);
		data->getValue().can->getValue().LP = stof(sp[6]);
		data->getValue().can->getValue().CP = stof(sp[7]);

		return data->getValue().can->length(); //RETURN SO NEN, =1
	}

	//NEU DA CO EX
	for (data->moveToStart(); !data->isLast(); data->next()) { //XET TUNG EX
		if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) { //NEU EX TRUNG
			for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) { //XET TUNG CA
				
				if (stoi(sp[3]) == data->getValue().can->getValue().TIME) return data->getValue().can->length(); //NEU CO TIME TRUNG -> KHONG CHEN
				else {
					if (stoi(sp[3]) < data->getValue().can->getValue().TIME) { //NEU TIM DUOC TIME LON HON -> INSERT BEFORE
						//data->getValue().can->prev();
						data->getValue().can->insertN();
						data->getValue().can->getValue().TIME = stoi(sp[3]);
						data->getValue().can->getValue().OP = stof(sp[4]);
						data->getValue().can->getValue().HP = stof(sp[5]);
						data->getValue().can->getValue().LP = stof(sp[6]);
						data->getValue().can->getValue().CP = stof(sp[7]);

						return data->getValue().can->length();
					}
				}
				if (data->getValue().can->isPreLast()) break;
			}
			if (data->getValue().can->isPreLast()) { //Neu di toi cuoi hang van khong co TIME nao trung
				// data->getValue().can->next();
				data->getValue().can->append0();
				data->getValue().can->getValue().TIME = stoi(sp[3]);
				data->getValue().can->getValue().OP = stof(sp[4]);
				data->getValue().can->getValue().HP = stof(sp[5]);
				data->getValue().can->getValue().LP = stof(sp[6]);
				data->getValue().can->getValue().CP = stof(sp[7]);

				return data->getValue().can->length();
			}
		}
		if (data->isPreLast()) break;
	}
	if (data->isPreLast()) {
		data->append0();
		data->getValue().BC = sp[1];
		data->getValue().QC = sp[2];
		data->getValue().can = new LList<Exchange::Candle>(); //TAO HEAD NODE CUA CA
		data->getValue().can->insert0();					//INSERT CA DAU TIEN
		data->getValue().can->getValue().TIME = stoi(sp[3]);
		data->getValue().can->getValue().OP = stof(sp[4]);
		data->getValue().can->getValue().HP = stof(sp[5]);
		data->getValue().can->getValue().LP = stof(sp[6]);
		data->getValue().can->getValue().CP = stof(sp[7]);

		return data->getValue().can->length();
	}

	return 1;

}

int ProcessData::del(const string* sp, const int n) {
	if (n < 3 || n > 5) return -1;
	int countC = 0;
	if (n == 3) {
		if (data == nullptr) return 0;

		for (data->moveToStart(); !data->isLast(); data->next()) { //XET TUNG EX
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) { //NEU EX TRUNG
				data->getValue().can->moveToStart();

				while (!data->getValue().can->isLast()) {
					data->getValue().can->remove();
					countC++;
				}

				return countC;
			}
		}
		return 0;
	}
	if (n == 4) {
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				data->getValue().can->moveToStart();
				while ( !data->getValue().can->isLast()) {
					if (data->getValue().can->getValue().TIME == stoi(sp[3])) {
						data->getValue().can->remove();
						return 1;
					}
					if (data->getValue().can->isPreLast()) break;
					data->getValue().can->next();
				}
				return countC;
			}
		}
		return 0;
	}
	if (n == 5) {
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
					if (stoi(sp[3]) <= data->getValue().can->getValue().TIME) break;//NEU CO TIME TRUNG -> KHONG CHEN	
				}
				while (stoi(sp[3]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[4])) {
					data->getValue().can->remove();
					countC++;
					if (data->getValue().can->isLast()) break;
				}

				
				return countC;
			}
		}
		return 0;
	}
	return 0;
}

int ProcessData::up(const string* sp, const int n) {
	if (n != 8) return -1;
	if (sp[4] > sp[5] || sp[4] < sp[6] || sp[7] > sp[5] || sp[7] < sp[6]) return -1;
	//BAN DAU KHONG CO EX NAO HET
	if (data == nullptr) return 0; //RETURN SO NEN, =1

	//NEU DA CO EX
	for (data->moveToStart(); !data->isLast(); data->next()) { //XET TUNG EX
		if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) { //NEU EX TRUNG
			for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) { //XET TUNG CA
				if (stoi(sp[3]) == data->getValue().can->getValue().TIME) {

					data->getValue().can->getValue().OP = stof(sp[4]);
					data->getValue().can->getValue().HP = stof(sp[5]);
					data->getValue().can->getValue().LP = stof(sp[6]);
					data->getValue().can->getValue().CP = stof(sp[7]);

					return 1;
				}
			}
			if (data->getValue().can->isLast()) { //Neu di toi cuoi hang van khong co TIME nao trung

				return 0;
			}
		}
	}
	if (data->isLast()) {


		return 0;
	}

	return 0;

}

int ProcessData::mb(const string* sp, const int n) {

	if (n < 3 || n > 5) return -1;
	int countC = 0;
	if (n == 3) {

		if (data == nullptr) return 0;

		for (data->moveToStart(); !data->isLast(); data->next()) { //XET TUNG EX
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) { //NEU EX TRUNG
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
					if (data->getValue().can->getValue().OP == data->getValue().can->getValue().HP &&
						data->getValue().can->getValue().CP == data->getValue().can->getValue().LP &&
						data->getValue().can->getValue().OP - data->getValue().can->getValue().CP > 0.0000501) {
						countC++;
					}
					else if (data->getValue().can->getValue().CP == data->getValue().can->getValue().HP && 
						data->getValue().can->getValue().OP == data->getValue().can->getValue().LP &&
						data->getValue().can->getValue().CP - data->getValue().can->getValue().OP > 0.0000501) {
						countC++;
					}
				}
				return countC;
			}
		}
		return 0;
	}
	if (n == 4) {
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
					if (data->getValue().can->getValue().TIME == stoi(sp[3])) {
						if (data->getValue().can->getValue().OP == data->getValue().can->getValue().HP &&
							data->getValue().can->getValue().CP == data->getValue().can->getValue().LP &&
							data->getValue().can->getValue().OP - data->getValue().can->getValue().CP > 0.0000501) {
							return 1;
						}
						else if (data->getValue().can->getValue().CP == data->getValue().can->getValue().HP &&
							data->getValue().can->getValue().OP == data->getValue().can->getValue().LP &&
							data->getValue().can->getValue().CP - data->getValue().can->getValue().OP > 0.0000501) {
							return 1;
						}

						// return countC;
					}
				}
				return 0;
			}
		}
		return 0;
	}
	if (n == 5) {
		//int a;
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
					if (stoi(sp[3]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[4])) {

						if (data->getValue().can->getValue().OP == data->getValue().can->getValue().HP &&
							data->getValue().can->getValue().CP == data->getValue().can->getValue().LP &&
							data->getValue().can->getValue().OP - data->getValue().can->getValue().CP > 0.0000501) {
							countC++;
						}
						else if (data->getValue().can->getValue().CP == data->getValue().can->getValue().HP &&
							data->getValue().can->getValue().OP == data->getValue().can->getValue().LP &&
							data->getValue().can->getValue().CP - data->getValue().can->getValue().OP > 0.0000501) {
							countC++;
						}

					}//NEU CO TIME TRUNG -> KHONG CHEN	
					if (data->getValue().can->getValue().TIME == stoi(sp[4])) break;
				}
				return countC;
			}
		}
		return 0;
	}
	return 0;
}

int ProcessData::st(const string* sp, const int n) {
	if (n < 3 || n > 5) return -1;
	int countC = 0;
	float a1, b1, c1, a2, b2, c2;
	if (n == 3) {

		if (data == nullptr) return 0;

		for (data->moveToStart(); !data->isLast(); data->next()) { //XET TUNG EX
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) { //NEU EX TRUNG
				data->getValue().can->moveToStart();
				while (!data->getValue().can->isLast()) {
					// return data->getValue().can->currPos();
					a1 = 10000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().CP);
					b1 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().OP);
					c1 = 10000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().LP);

					if ((a1 <= 0.501) && (a1 >= 0) && (b1 >= 0.501) && (c1 >= 0.501)) countC++;
					else {
						a2 = 10000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP);
						b2 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().CP);
						c2 = 10000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().LP);
						if ((a2 <= 0.501) && (a2 >= 0) && (b2 >= 0.501) && (c2 >= 0.501)) countC++;
					}
					data->getValue().can->next();
				}
				return countC;
			}
		}
		return 0;
	}
	if (n == 4) {
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {

					if (data->getValue().can->getValue().TIME == stoi(sp[3])) {
						a1 = 100000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().CP);
						b1 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().OP);
						c1 = 10000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().LP);
						if ((a1 <= 5.01) && (a1 <= 0) && (b1 >= 0.501) && (c1 >= 0.501)) return 1;
						else {
							a2 = 100000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP);
							b2 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().CP);
							c2 = 10000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().LP);
							if ((a2 <= 5.01) && (a2 >= 0) && (b2 >= 0.501) && (c2 >= 0.501)) return 1;
						}
						return 0;
					}

				}
				return 0;
			}
		}
		return 0;
	}
	if (n == 5) {
		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
					if (stoi(sp[3]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[4])) {
						
						a1 = 100000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().CP);
						b1 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().OP);
						c1 = 10000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().LP);
						if ((a1 <= 5.01) && (a1 >= 0) && (b1 >= 0.501) && (c1 >= 0.501)) countC++;
						else {
							a2 = 100000 * (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP);
							b2 = 10000 * (data->getValue().can->getValue().HP - data->getValue().can->getValue().CP);
							c2 = 10000 * (data->getValue().can->getValue().OP - data->getValue().can->getValue().LP);
							if (( a2 <= 5.01) && ( a2 >= 0) && ( b2 >= 0.501) && ( c2 >= 0.501)) countC++;
						}
						if (data->getValue().can->isPreLast()) break;
					}//NEU CO TIME TRUNG -> KHONG CHEN

				}
				return countC;
			}
		}
		return 0;
	}
	return 0;
}

int ProcessData::dj(const string* sp, const int n) {
	if (n < 3 || n > 6) return -1;
	int countC = 0;
	float a1, b1, c1, a2, b2, c2;
	if (n == 3) {
		if (data == nullptr) return 0;

		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				data->getValue().can->moveToStart();
				while (!data->getValue().can->isLast()) {
					if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209 && data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001)
						|| (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209 && data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001))

						countC++;
					data->getValue().can->next();
					if (data->getValue().can->isPreLast()) break;
				}
				return countC;
			}
			if (data->isPreLast()) break;
		}
		return 0;
	}
	if (n == 4) {
		if (sp[3] == "C1") { //TIM NEN
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isLast()) {
						if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) && 
							(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.0000001) &&
							(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501) &&
							(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) || 
							((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) && 
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.0000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {
								countC++;
						}
						if (data->getValue().can->isPreLast()) break;
						data->getValue().can->next();
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C2") {//TIM NEN
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isLast()) {
						if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209) &&
							(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
							(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP < 0.000009) &&
							(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) ||
							((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP < 0.000009) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {

							countC++;
						}
						if (data->getValue().can->isPreLast()) break;
						data->getValue().can->next();
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C3") {//TIM NEN
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isLast()) {
						if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) && 
							(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
							(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP < 0.000009) &&
							(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501)) || 
							((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) && 
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP < 0.000009) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501))) {
							
								countC++;
						} //DOJI BIA MO GIAM
						
						if (data->getValue().can->isPreLast()) break;
						
						data->getValue().can->next();
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C4") {//TIM NEN
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isLast()) {
						//DOJI BON GIA
						if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP < 0.000009)) {
							if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
								countC++;
						} //DOJI BON GIA GIAM
						else {
							if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP < 0.000009)) {
								if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
									countC++;
							} //DOJI BON GIA TANG
						}
						if (data->getValue().can->isPreLast()) break;
						data->getValue().can->next();
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (data->getValue().can->getValue().TIME == stoi(sp[3])) {

							if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209 && data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001)
								|| (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209 && data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001))

								return 1;
							
						}
						
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
	}
	if (n == 5) {
		//CODE VA TIME A
		if (sp[3] == "C1") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {

							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.0000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) ||
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) &&
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.0000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {
								return 1;
							}
						}
						if (data->getValue().can->isPreLast()) break;
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C2") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {

							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP < 0.000009) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) ||
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209) &&
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP < 0.000009) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {

								return 1;
							}
						}
						if (data->getValue().can->isPreLast()) break;
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C3") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {

							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP < 0.000009) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501)) ||
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) &&
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP < 0.000009) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501))) {

								return 1;
							}
						}
						if (data->getValue().can->isPreLast()) break;
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else if (sp[3] == "C4") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					data->getValue().can->moveToStart();
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {

							//DOJI BON GIA
							if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP < 0.000009)) {
								if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000009) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
									return 1;
							} //DOJI BON GIA GIAM
							else {
								if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP < 0.000009)) {
									if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000009) &&
										(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
										return 1;
								} //DOJI BON GIA TANG
							}
						}
						if (data->getValue().can->isPreLast()) break;

					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (stoi(sp[3]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[4])) {

							if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209 && data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) 
								|| (data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209 && data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001)) 
							
										countC++;
							
						}

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) break;
					}

					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
	}
	if (n == 6) {
		if (sp[3] == "C1") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {
							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.0000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) ||
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) &&
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.0000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {
								countC++;
							}
						
						}//NEU CO TIME TRUNG -> KHONG CHEN
						if (data->getValue().can->getValue().TIME == stoi(sp[5])) break;
					}
					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		if (sp[3] == "C2") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {
							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000209) && 
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP < 0.000009) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP > 0.0000501)) || 
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000209) && 
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP < 0.000009) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP > 0.0000501))) {
								
									countC++;
							} //DOJI CHUON CHUON GIAM
							
							//data->getValue().can->next();
						}//NEU CO TIME TRUNG -> KHONG CHEN

						if (data->getValue().can->getValue().TIME == stoi(sp[5])) break;
					}


					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		if (sp[3] == "C3") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {
							if (((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP <= 0.0000201) &&
								(data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
								(data->getValue().can->getValue().CP - data->getValue().can->getValue().LP < 0.000009) &&
								(data->getValue().can->getValue().HP - data->getValue().can->getValue().OP > 0.0000501)) ||
								((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP <= 0.0000201) &&
									(data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
									(data->getValue().can->getValue().OP - data->getValue().can->getValue().LP < 0.000009) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().CP > 0.0000501))) {

								countC++;
							}
						}//NEU CO TIME TRUNG -> KHONG CHEN

						if (data->getValue().can->getValue().TIME == stoi(sp[5])) break;
					}

					return countC;
					
				}
				if (data->isPreLast()) break;
			}
			return 0;
			
		}
		if (sp[3] == "C4") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					for (data->getValue().can->moveToStart(); !data->getValue().can->isLast(); data->getValue().can->next()) {
						if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {
							//DOJI BON GIA
							if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP < 0.000009)) {
								if ((data->getValue().can->getValue().OP - data->getValue().can->getValue().CP >= -0.000001) &&
									(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
									countC++;
							} //DOJI BON GIA GIAM
							else {
								if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP < 0.000009)) {
									if ((data->getValue().can->getValue().CP - data->getValue().can->getValue().OP >= -0.000001) &&
										(data->getValue().can->getValue().HP - data->getValue().can->getValue().LP < 0.000009))
										countC++;
								} //DOJI BON GIA TANG
							}
							
							//data->getValue().can->next();
						}//NEU CO TIME TRUNG -> KHONG CHEN

						if (data->getValue().can->getValue().TIME == stoi(sp[5])) break;
					}

					return countC;
				}
				if (data->isPreLast()) break;
			}
			return 0;
		}
		else return -1;//wrong syntax sp3
	}
	return -1;
}

int ProcessData::eg(const string* sp, const int n) {
	if (n < 3 || n > 6) return -1;
	int countC = 0;
	double a1, b1, c1, d1, a2, b2, c2, d2;
	if (n == 3) {
		if (data == nullptr) return 0;

		for (data->moveToStart(); !data->isLast(); data->next()) {
			if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
				if (data->getValue().can->length() == 1) return 0;
				data->getValue().can->moveToStart();
				while (!data->getValue().can->isPreLast()) {

					//DIEU KIEN CUM NHAN CHIM TANG
					
					if (((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) && 
						(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) && 
						(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) ||
						((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
							(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
							(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP )))
						countC++;
					
					data->getValue().can->next();
				}
				return countC;
			}
		}
		return 0;
	}
	if (n == 4) {
		if (sp[3] == "C1") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isPreLast()) {
						//DIEU KIEN CUM NHAN CHIM TANG
						
						if ((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) &&
							(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) &&
							(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) countC++;
						data->getValue().can->next();
					}

					return countC;
				}
			}
			return 0;
		}
		if (sp[3] == "C2") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					data->getValue().can->moveToStart();
					while (!data->getValue().can->isPreLast()) {
						//DIEU KIEN CUM NHAN CHIM GIAM
						
						if ((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
							(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
							(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP)) countC++;
						data->getValue().can->next();
					}
					return countC;
				}
			}
			return 0;
		}
		else {
			if (stoi(sp[3]) == -1) return -1;
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[3])) {
							
							if (((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) &&
								(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) &&
								(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) ||
								((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
									(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
									(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP)))
								countC++;
						}

					}
					return 0;
				}
			}
			return 0;
		}
	}
	if (n == 5) {
		if (sp[3] == "C1") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {
							
							if ((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) &&
								(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) &&
								(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) return 1;

						}

					}
					return 0;
				}
			}
			return 0;
		}
		if (sp[3] == "C2") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {

						if (data->getValue().can->getValue().TIME == stoi(sp[4])) {
							//DIEU KIEN CUM NHAN CHIM GIAM
							
							if ((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
								(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
								(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP)) return 1;

						}

					}
					return 0;
				}
			}
			return 0;
		}
		else {
			if (stoi(sp[3]) == -1) return -1;
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() == 1) return 0;
					for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {
						if (stoi(sp[3]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME < stoi(sp[4])) {
							
								//DIEU KIEN CUM NHAN CHIM TANG
							if (((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) &&
								(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) &&
								(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) ||
								((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
									(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
									(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP)))
								countC++;
								if (data->getValue().can->getValue().TIME == stoi(sp[4])) break;
							
						}//NEU CO TIME TRUNG -> KHONG CHEN

					}



					return countC;
				}
			}
			return 0;
		}
	}
	if (n == 6) {
		if (sp[3] == "C1") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() <= 1) return 0;
					break;
				}
			}
			for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {
				
				if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {

					if ((data->getValue().can->getValue().OP >= data->getValue().can->getValue().CP) &&
						(data->getValue().can->getValueNext().CP > data->getValue().can->getValue().OP) &&
						(data->getValue().can->getValue().CP > data->getValue().can->getValueNext().OP)) countC++;

				}//NEU CO TIME TRUNG -> KHONG CHEN
				if (data->getValue().can->getValue().TIME == stoi(sp[5])) return countC;;

			}

			return countC;
			return 0;
		}
		else if (sp[3] == "C2") {
			for (data->moveToStart(); !data->isLast(); data->next()) {
				if (sp[1] == data->getValue().BC && sp[2] == data->getValue().QC) {
					if (data->getValue().can->length() <= 1) return 0;
					break;
				}
			}
			for (data->getValue().can->moveToStart(); !data->getValue().can->isPreLast(); data->getValue().can->next()) {

				if (stoi(sp[4]) <= data->getValue().can->getValue().TIME && data->getValue().can->getValue().TIME <= stoi(sp[5])) {

					//DIEU KIEN CUM NHAN CHIM GIAM

					if ((data->getValue().can->getValue().CP >= data->getValue().can->getValue().OP) &&
						(data->getValue().can->getValueNext().OP > data->getValue().can->getValue().CP) &&
						(data->getValue().can->getValue().OP > data->getValue().can->getValueNext().CP)) countC++;

					//if (ProcessData::Exchange::Candle::CheckEGdwn(data->getValue().can->getValue().OP, data->getValue().can->getValue().CP, data->getValue().can->getValueNext().OP, data->getValue().can->getValueNext().CP)) countC++;

				}//NEU CO TIME TRUNG -> KHONG CHEN
				if (data->getValue().can->getValue().TIME == stoi(sp[5])) break;

			}
			return countC;
			return 0;
		}
		else return -1;
	}

	return 0;
}

//bool CheckEGdwn(List<E>*  ) {
//
//	if (op1 > cp1 && cp2 > op1 && cp1 > op2) return 1;
//	else return 0;
//}
//bool CheckEGup(float op1, float cp1, float op2, float cp2) {
//
//	if (cp1 > op1 && op2 > cp1 && op1 > cp2) return 1;
//	else return 0;
//}
