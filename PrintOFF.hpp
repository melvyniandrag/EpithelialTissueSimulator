void PrintOFF(int iter, vector<cell> sim_cells, vector<coordinate> coords, double* X, double* Y)
{
    ofstream OffFile; // Might be faster to pass this as an arguement to the function instead of instantiation.
    ostringstream ss;
    ss << setw(5) << setfill('0') << iter;
    string s2(ss.str());

    OffFile.open("Images/NagaiHonda"+ s2 +".off");
    OffFile << "{appearance {+edge}\n" <<"OFF" << endl;
    OffFile << coords.size() << " " << sim_cells.size() << " 0" << endl;
    for(auto pos:coords)
    {
        OffFile << X[pos.index] << " " << Y[pos.index] << " " << 0.00 << endl;
    }
    for(auto vert:sim_cells)
    {
        vert.PrintCell(OffFile);
    }
    OffFile << "}";
    OffFile.close();	
}
