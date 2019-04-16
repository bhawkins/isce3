//
// Author: Joshua Cohen
// Copyright 2017
//

#include <cmath>
#include <iostream>
#include "isce/cuda/core/gpuProjections.h"
#include "gtest/gtest.h"
using isce::core::cartesian_t;
using isce::cuda::core::UTM;

struct UTMTest : public ::testing::Test {
    virtual void SetUp() {
        fails = 0;
    }
    virtual void TearDown() {
        if (fails > 0) {
            std::cerr << "UTM::TearDown sees failures" << std::endl;
        }
    }
    unsigned fails;
};


#define utmTest(code,name,p,q,r,x,y,z)       \
    TEST_F(UTMTest, name) {       \
        UTM proj(code); \
        cartesian_t ref_llh = {p,q,r};    \
        cartesian_t ref_xyz = {x,y,z};    \
        cartesian_t xyz, llh;  \
        llh = ref_llh;                  \
        proj.forward_h(llh, xyz);    \
        EXPECT_NEAR(xyz[0], ref_xyz[0], 1.0e-6);\
        EXPECT_NEAR(xyz[1], ref_xyz[1], 1.0e-6);\
        EXPECT_NEAR(xyz[2], ref_xyz[2], 1.0e-6);\
        xyz = ref_xyz;                  \
        proj.inverse_h(xyz, llh);    \
        EXPECT_NEAR(llh[0], ref_llh[0], 1.0e-9);\
        EXPECT_NEAR(llh[1], ref_llh[1], 1.0e-9);\
        EXPECT_NEAR(llh[2], ref_llh[2], 1.0e-6);\
        fails += ::testing::Test::HasFailure();\
    } struct consume_semicolon


//Test origins for various northern systems
#define utmOriginNorthName(ind)  ind ## _NOrigin 
#define utmOriginNorthTest(x)\
    utmTest(32600+x, utmOriginNorthName(x), {(-177.0+(x-1)*6.0) * M_PI / 180.0, 0., 0.}, {500000., 0., 0.}) \

utmOriginNorthTest(1);
utmOriginNorthTest(2);
utmOriginNorthTest(3);
utmOriginNorthTest(4);
utmOriginNorthTest(5);
utmOriginNorthTest(6);
utmOriginNorthTest(7);
utmOriginNorthTest(8);
utmOriginNorthTest(9);
utmOriginNorthTest(10);
utmOriginNorthTest(11);
utmOriginNorthTest(12);
utmOriginNorthTest(13);
utmOriginNorthTest(14);
utmOriginNorthTest(15);
utmOriginNorthTest(16);
utmOriginNorthTest(17);
utmOriginNorthTest(18);
utmOriginNorthTest(19);
utmOriginNorthTest(20);
utmOriginNorthTest(21);
utmOriginNorthTest(22);
utmOriginNorthTest(23);
utmOriginNorthTest(24);
utmOriginNorthTest(25);
utmOriginNorthTest(26);
utmOriginNorthTest(27);
utmOriginNorthTest(28);
utmOriginNorthTest(29);
utmOriginNorthTest(30);
utmOriginNorthTest(31);
utmOriginNorthTest(32);
utmOriginNorthTest(33);
utmOriginNorthTest(34);
utmOriginNorthTest(35);
utmOriginNorthTest(36);
utmOriginNorthTest(37);
utmOriginNorthTest(38);
utmOriginNorthTest(39);
utmOriginNorthTest(40);
utmOriginNorthTest(41);
utmOriginNorthTest(42);
utmOriginNorthTest(43);
utmOriginNorthTest(44);
utmOriginNorthTest(45);
utmOriginNorthTest(46);
utmOriginNorthTest(47);
utmOriginNorthTest(48);
utmOriginNorthTest(49);
utmOriginNorthTest(50);
utmOriginNorthTest(51);
utmOriginNorthTest(52);
utmOriginNorthTest(53);
utmOriginNorthTest(54);
utmOriginNorthTest(55);
utmOriginNorthTest(56);
utmOriginNorthTest(57);
utmOriginNorthTest(58);
utmOriginNorthTest(59);
utmOriginNorthTest(60);


#define utmOriginSouthName(ind)  ind ## _SOrigin 
#define utmOriginSouthTest(x)\
    utmTest(32700+x, utmOriginSouthName(x), {(-177.0+(x-1)*6.0) * M_PI / 180.0, 0., 0.}, {500000., 10000000., 0.})

utmOriginSouthTest(1);
utmOriginSouthTest(2);
utmOriginSouthTest(3);
utmOriginSouthTest(4);
utmOriginSouthTest(5);
utmOriginSouthTest(6);
utmOriginSouthTest(7);
utmOriginSouthTest(8);
utmOriginSouthTest(9);
utmOriginSouthTest(10);
utmOriginSouthTest(11);
utmOriginSouthTest(12);
utmOriginSouthTest(13);
utmOriginSouthTest(14);
utmOriginSouthTest(15);
utmOriginSouthTest(16);
utmOriginSouthTest(17);
utmOriginSouthTest(18);
utmOriginSouthTest(19);
utmOriginSouthTest(20);
utmOriginSouthTest(21);
utmOriginSouthTest(22);
utmOriginSouthTest(23);
utmOriginSouthTest(24);
utmOriginSouthTest(25);
utmOriginSouthTest(26);
utmOriginSouthTest(27);
utmOriginSouthTest(28);
utmOriginSouthTest(29);
utmOriginSouthTest(30);
utmOriginSouthTest(31);
utmOriginSouthTest(32);
utmOriginSouthTest(33);
utmOriginSouthTest(34);
utmOriginSouthTest(35);
utmOriginSouthTest(36);
utmOriginSouthTest(37);
utmOriginSouthTest(38);
utmOriginSouthTest(39);
utmOriginSouthTest(40);
utmOriginSouthTest(41);
utmOriginSouthTest(42);
utmOriginSouthTest(43);
utmOriginSouthTest(44);
utmOriginSouthTest(45);
utmOriginSouthTest(46);
utmOriginSouthTest(47);
utmOriginSouthTest(48);
utmOriginSouthTest(49);
utmOriginSouthTest(50);
utmOriginSouthTest(51);
utmOriginSouthTest(52);
utmOriginSouthTest(53);
utmOriginSouthTest(54);
utmOriginSouthTest(55);
utmOriginSouthTest(56);
utmOriginSouthTest(57);
utmOriginSouthTest(58);
utmOriginSouthTest(59);
utmOriginSouthTest(60);


#define utmNorthName(ind)  ind ## _North
#define utmNorthTest(code,p,q,r,x,y,z)\
    utmTest(32600+code, utmNorthName(code), p, q, r, x, y, z)


utmNorthTest(1, {-3.090970668593823e+00,   3.623975597550786e-02, 1.744676256950977e+03},
        {  4.889271377427764e+05,   2.295042945422887e+05, 1.744676256950977e+03});

utmNorthTest(2, { -2.953263114726996e+00,   9.600379960214160e-01, 1.309856754109411e+03},
        {  6.145110544533111e+05,   6.096939002906235e+06, 1.309856754109411e+03});

utmNorthTest(3, { -2.828409865931200e+00,   6.877340694823902e-02, 1.408144081790089e+03},
        {  8.269735216827290e+05,   4.361200141956932e+05, 1.408144081790089e+03});

utmNorthTest(4, { -2.762855676728190e+00,   1.319614665396293e+00, 2.100746586333807e+03},
        {  5.194215784019039e+05,   8.391599518107835e+06, 2.100746586333807e+03});

utmNorthTest(5, { -2.629822233787476e+00,   7.101934154796464e-03, -4.030328978783106e+02},
        {  7.584769701424806e+05,   4.501309978549536e+04, -4.030328978783106e+02});

utmNorthTest(6, { -2.553777190333096e+00,   3.551856335778905e-01, 1.255025776155367e+03},
        {  5.709056491537427e+05,   2.250429843888853e+06, 1.255025776155367e+03});

utmNorthTest(7, { -2.478164433583644e+00,   1.066561900213198e+00, 6.928509000142972e+02},
        {  4.467290760585882e+05,   6.775385850477098e+06, 6.928509000142972e+02});

utmNorthTest(8, { -2.346013584676602e+00,   5.880768215307526e-01, 1.572481477086877e+03},
        {  5.540611572555148e+05,   3.728416288221708e+06, 1.572481477086877e+03});

utmNorthTest(9, { -2.265194260628123e+00,   1.236884024142398e+00, 6.881004461911346e+01},
        {  4.712470996209013e+05,   7.862887424353285e+06, 6.881004461911346e+01});

utmNorthTest(10, { -2.196477359059586e+00,   4.229039715469926e-01, -2.168384271992239e+02},
        {  2.106761979968518e+05,   2.682712654303357e+06, -2.168384271992239e+02});

utmNorthTest(11, { -1.998718630180417e+00,   5.713803956491203e-02, 7.439376237374490e+02},
        {  7.758072634170417e+05,   3.621950283763906e+05, 7.439376237374490e+02});

utmNorthTest(12, { -1.919039258913648e+00,   4.660539917897892e-01, 2.048701915580112e+03},
        {  6.041682534683502e+05,   2.953960121754555e+06, 2.048701915580112e+03});

utmNorthTest(13, { -1.802271678436952e+00,   7.642377893267469e-01, 1.623465287535497e+02},
        {  6.397945652097389e+05,   4.849749410946064e+06, 1.623465287535497e+02});

utmNorthTest(14, { -1.699947067261184e+00,   1.157401543873968e+00, 5.057363325972399e+02},
        {  5.717268931380039e+05,   7.355852354796493e+06, 5.057363325972399e+02});

utmNorthTest(15, { -1.590825810117741e+00,   9.377197868855468e-01, 1.529661434922360e+03},
        {  6.222092278017082e+05,   5.954784143304377e+06, 1.529661434922360e+03});

utmNorthTest(16, { -1.550534790058901e+00,   1.016016615134610e-02, 1.654989912131431e+03},
        {  2.953293895502636e+05,   6.437677300055693e+04, 1.654989912131431e+03});

utmNorthTest(17, { -1.422255070590467e+00,   1.039374368583357e+00, -4.348629180687368e+01},
        {  4.723447267035230e+05,   6.601595736134732e+06, -4.348629180687368e+01});

utmNorthTest(18, { -1.342006238831657e+00,   5.029413925486856e-01, 1.653909605974295e+03},
        {  3.154455139877747e+05,   3.189115238952604e+06, 1.653909605974295e+03});

utmNorthTest(19, { -1.180489367837085e+00,   1.165256977450649e+00, 1.687624423204597e+03},
        {  5.599984784588905e+05,   7.405761860478484e+06, 1.687624423204597e+03});

utmNorthTest(20, { -1.075294565748616e+00,   7.138741851966671e-01, -4.653923934317306e+02},
        {  6.170892534184410e+05,   4.528805942272532e+06, -4.653923934317306e+02});

utmNorthTest(21, { -1.020348517132277e+00,   5.924698624028456e-01, 1.298085855967110e+03},
        {  3.649276518876015e+05,   3.757133312543859e+06, 1.298085855967110e+03});

utmNorthTest(22, { -9.361607448138209e-01,   9.208884921763545e-01, 1.904019485003505e+03},
        {  3.220078156709126e+05,   5.849172719771276e+06, 1.904019485003505e+03});

utmNorthTest(23, { -8.120470039671042e-01,   3.272706775342283e-01, 5.444127873367556e+02},
        {  3.390449851274882e+05,   2.073991340078732e+06, 5.444127873367556e+02});

utmNorthTest(24, { -6.384386186270904e-01,   7.615647785534717e-01, 6.823877679743382e+02},
        {  6.952245129789326e+05,   4.834119100372418e+06, 6.823877679743382e+02});

utmNorthTest(25, { -5.934180522797425e-01,   3.148956113256793e-01, 1.336968253872159e+03},
        {  3.941211748382293e+05,   1.995139525815777e+06, 1.336968253872159e+03});

utmNorthTest(26, { -4.193368414537194e-01,   8.175280921983901e-01, 1.480439902059571e+03},
        {  7.267462549247982e+05,   5.191779406219411e+06, 1.480439902059571e+03});

utmNorthTest(27, { -3.948180036555322e-01,   2.314527049040553e-01, 1.258250047807526e+03},
        {  3.243372220250848e+05,   1.466598393050579e+06, 1.258250047807526e+03});

utmNorthTest(28, { -3.097376781049765e-01,   1.671934545983628e-01, 1.639637182875109e+03},
        {  1.984894008299313e+05,   1.060122187508237e+06, 1.639637182875109e+03});

utmNorthTest(29, { -1.895040932817424e-01,   6.620427499751563e-03, 1.766626084527058e+03},
        {  2.932430981645094e+05,   4.194874294445351e+04, 1.766626084527058e+03});

utmNorthTest(30, { -1.752847334938723e-02,   1.334490122292536e+00, -4.723082040943909e+02},
        {  5.521455114972482e+05,   8.487470389155295e+06, -4.723082040943909e+02});

utmNorthTest(31, {  6.022337844354532e-02,   1.211724142598408e+00, -1.113257705672240e+02},
        {  5.176693033186447e+05,   7.702005036426845e+06, -1.113257705672240e+02});

utmNorthTest(32, {  1.065396559550746e-01,   1.043208094600939e+00, 1.411896832705306e+03},
        {  3.374042813130312e+05,   6.629506544646490e+06, 1.411896832705306e+03});

utmNorthTest(33, {  2.968214703350750e-01,   1.113741065268409e+00, 2.467613981777665e+03},
        {  5.987922661240866e+05,   7.077691508892422e+06, 2.467613981777665e+03});

utmNorthTest(34, {  3.652589929715603e-01,   4.144743577124118e-01, -3.187872693742801e+02},
        {  4.926420797126418e+05,   2.626288149434630e+06, -3.187872693742801e+02});

utmNorthTest(35, {  4.358123299137315e-01,   7.611988592521465e-01, 2.423951558018800e+03},
         {  3.362087111580424e+05,   4.830946302488425e+06, 2.423951558018800e+03});

utmNorthTest(36, {  6.074483161680040e-01,   4.841156938475057e-01, 1.504563719508563e+03},
        {  6.778402668492512e+05,   3.069459643796592e+06, 1.504563719508563e+03});

utmNorthTest(37, {  6.444460230329779e-01,   1.342714916223128e+00, 1.805942566481697e+03},
        {  4.476118123803679e+05,   8.540098114223808e+06, 1.805942566481697e+03});

utmNorthTest(38, {  7.398767550003145e-01,   6.661201141991332e-01, 1.395743947351115e+03},
        {  2.715062737933492e+05,   4.227433993356098e+06, 1.395743947351115e+03});

utmNorthTest(39, {  8.918913979362174e-01,   1.012096003501227e+00, 2.277824540199285e+03},
        {  5.060081262420265e+05,   6.427471027060431e+06, 2.277824540199285e+03});

utmNorthTest(40, {  9.684415065215669e-01,   1.156631290653948e+00, 1.745101840015008e+03},
        {  4.320899177967507e+05,   7.350836297757769e+06, 1.745101840015008e+03});

utmNorthTest(41, {  1.064335667633314e+00,   3.455513253421785e-01, 1.247755962405140e+03},
        {  2.885995077347172e+05,   2.190459622794821e+06, 1.247755962405140e+03});

utmNorthTest(42, {  1.242072453451163e+00,   2.345260094176481e-01, 9.492404262246669e+02},
        {  7.344629210203595e+05,   1.486531487748360e+06, 9.492404262246669e+02});

utmNorthTest(43, {  1.342079068537992e+00,   3.644833862692313e-01, 3.070785025139514e+02},
        {  6.971732184236289e+05,   2.310401723937681e+06, 3.070785025139514e+02});

utmNorthTest(44, {  1.464444952904991e+00,   1.149931035769693e+00, -1.598382707226714e+02},
        {  6.324664766427216e+05,   7.310292696183661e+06, -1.598382707226714e+02});

utmNorthTest(45, {  1.543419483837435e+00,   1.381085987774121e+00, 1.885841185018352e+03},
        {  5.301309383707922e+05,   8.784897241369683e+06, 1.885841185018352e+03});

utmNorthTest(46, {  1.585423157074981e+00,   1.162017907248301e+00, 4.433953522282509e+02},
        {  4.041209520092876e+05,   7.386078571491429e+06, 4.433953522282509e+02});

utmNorthTest(47, {  1.728848061236068e+00,   6.646558535694312e-01, -1.364991080934421e+02},
        {  5.048846283413017e+05,   4.214911870909736e+06, -1.364991080934421e+02});

utmNorthTest(48, {  1.781773982638371e+00,   1.207480146502244e+00, 6.581028943527792e+02},
        {  3.845506310335514e+05,   7.677568157483398e+06, 6.581028943527792e+02});

utmNorthTest(49, {  1.935231079749878e+00,   1.282203022449893e-01, 1.138943317254907e+03},
        {  4.868191530416262e+05,   8.120533103041976e+05, 1.138943317254907e+03});

utmNorthTest(50, {  2.069545360273451e+00,   4.078500463543793e-01, 5.606716766039922e+02},
        {  6.611055940246871e+05,   2.585146869307164e+06, 5.606716766039922e+02});

utmNorthTest(51, {  2.120140094423956e+00,   2.552172843315898e-01, 2.674026938045199e+02},
        {  3.357588175678421e+05,   1.617166655843936e+06, 2.674026938045199e+02});

utmNorthTest(52, {  2.200596735058002e+00,   5.651345096443301e-01, -2.543097767618632e+02},
        {  2.257440208521659e+05,   3.586275646175082e+06, -2.543097767618632e+02});

utmNorthTest(53, {  2.379744015943484e+00,   3.473288596827731e-01, -3.962862158003024e+02},
        {  6.412412143420798e+05,   2.201034403458965e+06, -3.962862158003024e+02});

utmNorthTest(54, {  2.482331105192906e+00,   1.107532267248292e+00, -4.405417883545738e+00},
        {  5.611792502134715e+05,   7.037086180304332e+06, -4.405417883545738e+00});

utmNorthTest(55, {  2.545312216356774e+00,   1.318057316565008e+00, 2.040338384576626e+03},
        {  4.675016240815290e+05,   8.381848172357652e+06, 2.040338384576626e+03});

utmNorthTest(56, {  2.684893927162709e+00,   9.378611954626423e-01, 8.709687532532478e+02},
        {  5.549537192903002e+05,   5.954414751003715e+06, 8.709687532532478e+02});

utmNorthTest(57, {  2.786366442192657e+00,   8.821049568552628e-01, 2.409703153002773e+03},
        {  5.458487548545831e+05,   5.598972147518563e+06, 2.409703153002773e+03});

utmNorthTest(58, {  2.908644712059227e+00,   6.194710989390468e-01, 1.882995490264797e+03},
        {  6.499413839613426e+05,   3.928981995973092e+06, 1.882995490264797e+03});

utmNorthTest(59, {  2.988940641526327e+00,   7.054352553468708e-01, 1.123442923990343e+03},
        {  5.215216360746034e+05,   4.474235076855149e+06, 1.123442923990343e+03});

utmNorthTest(60, {  3.139969074874963e+00,   1.090397747278831e+00, 4.221467180650762e+02},
        {  6.498459590456143e+05,   6.930490576489620e+06, 4.221467180650762e+02});



#define utmSouthName(ind)  ind ## _South
#define utmSouthTest(code,p,q,r,x,y,z)\
    utmTest(32700+code, utmSouthName(code), p, q, r, x, y, z)

utmSouthTest(1, {-3.093927794291543e+00,  -1.067144322057127e+00, 4.748737687030280e+01},
        {  4.855160732325171e+05,   3.221269661690438e+06, 4.748737687030280e+01});

utmSouthTest(2, { -2.996139611250702e+00,  -1.152056701743504e+00, 2.406091576565912e+03},
        {  4.697754426835759e+05,   2.679039200865542e+06, 2.406091576565912e+03});

utmSouthTest(3, { -2.876914594079851e+00,  -7.466818577552757e-01, 2.406658856071156e+03},
        {  5.134911320872664e+05,   5.263411274956161e+06, 2.406658856071156e+03});

utmSouthTest(4, { -2.772727754086391e+00,  -9.547483512334889e-01, 2.184106949898696e+03},
        {  5.086608712734164e+05,   3.938243661063939e+06, 2.184106949898696e+03});

utmSouthTest(5, { -2.690359919284988e+00, -9.317819910083976e-01, 8.303071193641349e+02},
        {  4.237648695405290e+05,   4.084045829159511e+06, 8.303071193641349e+02});

utmSouthTest(6, { -2.537236614050413e+00, -1.299939031206485e-01, 1.446826238341395e+02},
        {  6.795559742593870e+05,   9.176383981650868e+06, 1.446826238341395e+02});

utmSouthTest(7, { -2.511083462518575e+00, -1.110266591339036e-01, 1.340706052745041e+03},
        {  1.819671596542899e+05,   9.295965274186375e+06, 1.340706052745041e+03});

utmSouthTest(8, { -2.336503240146032e+00, -1.297602804282943e+00, 1.601696508755833e+02},
        {  5.339763011775943e+05,   1.748908272822330e+06, 1.601696508755833e+02});

utmSouthTest(9, { -2.221472224108557e+00, -1.071422169556050e+00, 3.676092284724679e+02},
        {  5.918309151826508e+05,   3.192788235917090e+06, 3.676092284724679e+02});

utmSouthTest(10, { -2.189935301639191e+00, -1.123044156234450e+00, 3.366606485924839e+02},
        {  3.805092971402773e+05,   2.862137678803152e+06, 3.366606485924839e+02});

utmSouthTest(11, { -1.999338819366139e+00, -5.257272992337476e-01, -3.528781311973934e+02},
        {  7.356890475149462e+05,   6.665174867217660e+06, -3.528781311973934e+02});

utmSouthTest(12, { -1.912609274811218e+00, -1.100329300197505e+00, 1.432919891429743e+02},
        {  5.715889865099670e+05,   3.008695027860953e+06, 1.432919891429743e+02});

utmSouthTest(13, { -1.862421005258392e+00, -8.082374424417190e-01, 9.906453169362928e+02},
        {  3.684171653585196e+05,   4.870245584529622e+06, 9.906453169362928e+02});

utmSouthTest(14, { -1.761746995910456e+00, -3.411428773349856e-01, 3.342050034809093e+02},
        {  2.963900278069759e+05,   7.837597879128788e+06, 3.342050034809093e+02});

utmSouthTest(15, { -1.656504734730955e+00, -5.792569785010917e-01, 2.469057495560771e+03},
        {  3.218757159113383e+05,   6.326136183659219e+06, 2.469057495560771e+03});

utmSouthTest(16, { -1.504385303603212e+00, -6.387562913532495e-01, 1.356167621293743e+03},
        {  5.720080703351937e+05,   5.949415262161504e+06, 1.356167621293743e+03});

utmSouthTest(17, { -1.431956753451354e+00, -8.973255961375410e-01, 1.823454523924874e+03},
        {  4.273210530589920e+05,   4.303731574196601e+06, 1.823454523924874e+03});

utmSouthTest(18, { -1.260854388194242e+00, -8.612324868672039e-01, 6.136067694401497e+02},
        {  7.003450553089906e+05,   4.530532605235888e+06, 6.136067694401497e+02});

utmSouthTest(19, { -1.202368715348580e+00, -1.106468200032539e+00, 1.148127342213503e+03},
        {  5.054635608365003e+05,   2.970288397763194e+06, 1.148127342213503e+03});

utmSouthTest(20, { -1.127024233564887e+00, -9.413357845544421e-01, 1.140500845932666e+03},
        {  3.966848609774373e+05,   4.022611284952912e+06, 1.140500845932666e+03});

utmSouthTest(21, { -9.738118081516539e-01, -1.370505788237530e+00, 1.528977172524021e+03},
        {  5.267545694514676e+05,   1.282852957568752e+06, 1.528977172524021e+03});

utmSouthTest(22, { -9.017302026233195e-01, -7.419156893245362e-01, 2.261173026528268e+03},
        {  4.453394231016677e+05,   5.293533345352869e+06, 2.261173026528268e+03});

utmSouthTest(23, { -7.816254772561744e-01, -6.904626922868485e-01, -3.677898652102435e+02},
        {  5.185689963501085e+05,   5.620987923941847e+06, -3.677898652102435e+02});

utmSouthTest(24, { -6.891602219533987e-01, -1.881748443736723e-01, 1.119211710586989e+03},
        {  4.468712468581963e+05,   8.808124138108084e+06, 1.119211710586989e+03});

utmSouthTest(25, { -5.416955304732076e-01, -6.567090015907744e-01, 1.403121877547386e+03},
        {  6.732366153572414e+05,   5.833794866212438e+06, 1.403121877547386e+03});

utmSouthTest(26, { -4.734668390688749e-01, -6.958179578577736e-01, 2.469945445051182e+03},
        {  4.890826525224931e+05,   5.586948535366161e+06, 2.469945445051182e+03});

utmSouthTest(27, { -3.632113851120634e-01, -6.807650787133717e-01, 1.612975167630004e+03},
         {  5.164097768657281e+05,   5.682655269341918e+06, 1.612975167630004e+03});

utmSouthTest(28, { -2.524826537539908e-01, -9.148045452735427e-01, 6.691271711099587e+02},
        {  5.363068004145084e+05,   4.192730986826724e+06, 6.691271711099587e+02});

utmSouthTest(29, { -1.530648268851086e-01, -4.479665742259433e-01, 8.126779789484167e+02},
        {  5.230856411332940e+05,   7.161217839428619e+06, 8.126779789484167e+02});

utmSouthTest(30, { -1.097367658216802e-02, -4.006143174766544e-01, 2.618928483722603e+02},
        {  7.431415710624452e+05,   7.459663782201533e+06, 2.618928483722603e+02});

utmSouthTest(31, { 5.165747510746871e-02, -1.124778746154647e+00, 2.308906013415342e+03},
        {  4.980629153514457e+05,   2.853387760201799e+06, 2.308906013415342e+03});

utmSouthTest(32, { 1.655882313277448e-01, -5.540498983696594e-02, 9.775833536700648e+02},
        {  5.541652696357307e+05,   9.649109090424379e+06, 9.775833536700648e+02});

utmSouthTest(33, { 2.333064499344660e-01, -2.525196264787167e-01, 4.332142720037806e+02},
        {  3.240442325626509e+05,   8.399853630859971e+06, 4.332142720037806e+02});

utmSouthTest(34, { 3.162014383393963e-01, -5.802509663967143e-01, 1.887441229611180e+03},
        {  2.313869800247925e+05,   6.317741679258667e+06, 1.887441229611180e+03});

utmSouthTest(35, { 5.213292608404462e-01, -6.567961165919739e-01, 8.308522921658482e+02},
        {  7.532577045564674e+05,   5.831178700238938e+06, 8.308522921658482e+02});

utmSouthTest(36, { 5.645960247093271e-01, -1.137883137660682e-01, 1.261919539264205e+03},
        {  4.280204477305153e+05,   9.279311816429880e+06, 1.261919539264205e+03});

utmSouthTest(37, { 6.413712891332739e-01, -3.828379725752712e-01, 6.811804842773352e+02},
        {  2.673837595218066e+05,   7.572659196159396e+06, 6.811804842773352e+02});

utmSouthTest(38, { 8.194429134812473e-01, -1.222013780614573e+00, 1.452908975235134e+03},
        {  5.743887274793803e+05,   2.231126525492365e+06, 1.452908975235134e+03});

utmSouthTest(39, { 9.278864474977708e-01, -1.154245095982212e+00, 3.074276236220485e+02},
        {  5.976866827747095e+05,   2.663536482581806e+06, 3.074276236220485e+02});

utmSouthTest(40, { 1.042275506399661e+00, -1.099125959582243e+00, 1.949717194480670e+03},
        {  6.377590864759501e+05,   3.014253669298320e+06, 1.949717194480670e+03});

utmSouthTest(41, { 1.149255210321273e+00, -2.898833946982658e-01, 1.796418458975198e+02},
        {  8.038209201088424e+05,   8.161528731460822e+06, 1.796418458975198e+02});

utmSouthTest(42, { 1.206414340897666e+00, -2.983845122738013e-02, 1.624760038170414e+03},
        {  5.136196136129121e+05,   9.811034921276949e+06, 1.624760038170414e+03});

utmSouthTest(43, { 1.341181926469192e+00, -8.076248783774596e-01, 7.386701048471368e+02},
        {  6.420839189316017e+05,   4.873911786792137e+06, 7.386701048471368e+02});

utmSouthTest(44, { 1.436594736770568e+00, -6.487289722045946e-01, 1.792449668144785e+03},
        {  6.163746600470975e+05,   5.885527336441822e+06, 1.792449668144785e+03});

utmSouthTest(45, { 1.533229227198443e+00, -9.414614123473218e-01, 1.311633755254112e+03},
        {  5.556343064680733e+05,   4.022624835578744e+06, 1.311633755254112e+03});

utmSouthTest(46, { 1.660705879782899e+00, -5.935309491351594e-01, 1.920828596127962e+03},
        {  6.986823748802551e+05,   6.235001138283439e+06, 1.920828596127962e+03});

utmSouthTest(47, { 1.759608453620377e+00, -4.368256617553442e-01, -1.810230889635023e+02},
        {  6.834456854345885e+05,   7.230690655819882e+06, -1.810230889635023e+02});

utmSouthTest(48, { 1.819195002486214e+00, -6.049461208245023e-01, 4.761826597265330e+02},
        {  4.296479956031065e+05,   6.164297176642289e+06, 4.761826597265330e+02});

utmSouthTest(49, { 1.894064030705631e+00, -1.181435798905016e-01, 1.385923243118906e+03},
         {  2.260726346747838e+05,   9.251074484218949e+06, 1.385923243118906e+03});

utmSouthTest(50, { 2.092301986861480e+00, -3.268962891175976e-01, 1.984957429894415e+03},
        {  8.037155830414421e+05,   7.926619027254893e+06, 1.984957429894415e+03});

utmSouthTest(51, { 2.148580616112694e+00, -1.560100419801283e-01, 1.587450180536730e+03},
        {  5.114990746568168e+05,   9.011921461862508e+06, 1.587450180536730e+03});

utmSouthTest(52, { 2.231123284515521e+00, -8.529693908699850e-01, 2.232721219345072e+03},
        {  4.144936743956888e+05,   4.586168238395445e+06, 2.232721219345072e+03});

utmSouthTest(53, { 2.328146846119682e+00, -1.005058375802772e+00, 1.260945397690811e+03},
        {  4.039213100553773e+05,   3.616286839631864e+06, 1.260945397690811e+03});

utmSouthTest(54, { 2.433932664761299e+00, -2.718814813122007e-01, -5.684495328850812e+01},
        {  3.342381537372246e+05,   8.277179815335035e+06, -5.684495328850812e+01});

utmSouthTest(55, { 2.532119916437567e+00, -9.627507128708438e-01, 1.105065529950088e+03},
        {  3.776686005437823e+05,   3.885548170682712e+06, 1.105065529950088e+03});

utmSouthTest(56, { 2.637858740523431e+00, -7.561723962960939e-01, 1.081645553960545e+03},
        {  3.490474175406274e+05,   5.201356265293564e+06, 1.081645553960545e+03});

utmSouthTest(57, { 2.734520249198158e+00, -9.192882174841261e-01, 2.093235387779316e+02},
        {  3.428968343171544e+05,   4.161756086536823e+06, 2.093235387779316e+02});

utmSouthTest(58, { 2.861230519195824e+00, -3.571713545716350e-01, 1.452687418735403e+03},
        {  3.890703978262835e+05,   7.736765598154459e+06, 1.452687418735403e+03});

utmSouthTest(59, { 2.959352714231888e+00, -1.074158844609437e+00, 1.991679650882862e+03},
        {  4.233742184432955e+05,   3.175683850020764e+06, 1.991679650882862e+03});

utmSouthTest(60, { 3.038341419519374e+00, -8.883583150753551e-01, 1.479453617383727e+03},
        {  2.949702298669473e+05,   4.357336082772384e+06, 1.479453617383727e+03});

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
